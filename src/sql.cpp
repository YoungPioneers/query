/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     Tue 16 Jun 2015 04:39:55 PM CST
*  File:     sql.cpp
*  Desc:
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <exception>

#include "sql.h"
#include "util.h"

using namespace std;

// only know simple select grammar
// example select id from example.csv where id > 10 and id < 100;
bool sql_init(const string &query, sql &result) {
	try {
		vector<string> items = split(query, SPACE_STR);
		size_t size = items.size();
		size_t select_pos(size), from_pos(size), where_pos(size), group_pos(size), order_pos(size);

		// find positions of keywords
		find_keyword_pos(items, select_pos, from_pos, where_pos, group_pos, order_pos);

		// syntax check
		if(!syntax_check(size, select_pos, from_pos, where_pos, group_pos, order_pos)) {
			return false;
		}

		// slice different type of elements from items
		vector<string> fields(items.begin() + select_pos + 1, items.begin() + from_pos);
		vector<string> table(items.begin() + from_pos + 1, items.begin() + where_pos);
		vector<string> conditions(items.begin() + where_pos + 1, items.begin() + group_pos);

		// deal with fields
		fields = split(join(fields, BLANK_STR), COMMA_STR);
		result.select = get_fields(fields);

		// decide whether table name is a file
		if(1 != table.size()) {
			return false;
		}
		string table_name = table.at(0);
		result.from = table_name;
		result.from_type = FROM_STDIN_STR == table_name;

		// deal with conditions
		vector<condition> where = get_where(conditions);
		sort(where.begin(), where.end(), [](condition c1, condition c2){ return c1.level < c2.level; });

		// deal with groups
		result.group_by = vector<string>();
		if(group_pos < size) {
			vector<string> groups(items.begin() + group_pos + 1, items.begin() + order_pos);
			if(BY_STR != upper(groups.at(0))) {
				return false;
			}
			result.group_by = get_group(groups);

		}

		// deal with orders
		result.order_by = vector<order>();
		if(order_pos < size) {
			vector<string> orders(items.begin() + order_pos + 1, items.begin() + size);
			if(BY_STR != upper(orders.at(0))) {
				return false;
			}
			result.order_by = get_order(orders);

		}

		return true;

	}
	catch(exception &e) {
		cout << e.what() << endl;
		return false;
	}
}

void find_keyword_pos(vector<string> items, size_t &select_pos, size_t &from_pos, size_t &where_pos, size_t &group_pos, size_t &order_pos) {
	size_t size = items.size(), pos = 0;

	// walk through items
	// find positions of all key words
	while(pos < size) {
		string item = upper(items.at(pos));

		if(SELECT_STR == item) {
			select_pos = pos;
		}
		else if(FROM_STR == item) {
			from_pos = pos;
		}
		else if(WHERE_STR == item) {
			where_pos = pos;
		}
		else if(GROUP_STR == item) {
			group_pos = pos;
		}
		else if(ORDER_STR == item) {
			order_pos = pos;
		}
		++pos;
	}
}

bool syntax_check(const size_t &size, const size_t &select_pos, const size_t &from_pos, const size_t &where_pos, const size_t &group_pos, const size_t &order_pos) {
	bool result = select_pos < size && from_pos < size && select_pos < from_pos;
	if(group_pos < size) {
		result  = result && where_pos < group_pos;
	}

	if(order_pos < size) {
		result  = result && where_pos < order_pos;
	}

	if(group_pos < size && order_pos < size) {
		result = result && group_pos < order_pos;
	}

	return result;
}

vector<field> get_fields(vector<string> fields) {
	vector<field> select = vector<field>();

	vector<string>::iterator it = fields.begin();
	while(fields.end() != it) {
		// 提取最外层括号
		// count (distince(id)) / name
		string field_raw = *it;

		// 解析嵌套的函数调用
		size_t left, right;
		unsigned int function_type = SELECT_PLAIN;
		string function_name(BLANK_STR), field_name(BLANK_STR);
		while(true) {
			left = field_raw.find(LEFT_BRACKET, 0);
			right = field_raw.rfind(RIGHT_BRACKET, field_raw.size());

			if(string::npos == left || string::npos == right) {
				// 函数最里层
				field_name = field_raw;

				field f = field();
				f.name = field_name;
				f.type = function_type;
				select.push_back(f);
				break;
			}
			function_name = upper(field_raw.substr(0, left));
			field_name = field_raw.substr(left + 1, field_raw.size() - left - 2);

			if(SUM_STR == function_name) {
				function_type |= SELECT_SUM;

			}
			else if(MAX_STR == function_name) {
				function_type |= SELECT_MAX;

			}
			else if(MIN_STR == function_name) {
				function_type |= SELECT_MIN;

			}
			else if(AVG_STR == function_name) {
				function_type |= SELECT_AVG;

			}
			else if(COUNT_STR == function_name) {
				function_type |= SELECT_COUNT;

			}
			else if(DISTINCT_STR == function_name) {
				function_type |= SELECT_DISTINCT;

			}

			field_raw = field_name;

		}

		++it;
	}

	return select;
}

vector<condition> get_where(vector<string> conditions) {
	// example : name = 'abc' or (id > 10 and id < 20)
	vector<condition> where = vector<condition>();
	vector<string>::iterator it = conditions.begin();

	bool logic = LOGIC_AND, is_op1 = true;
	unsigned int level = 0;
	string operator1(BLANK_STR), operator2(BLANK_STR), operation(BLANK_STR);
	// 解析嵌套的条件语句
	while(conditions.end() != it) {
		string item = *it;

		if(LEFT_BRACKET == item) {
			++level;
			logic = LOGIC_AND;

		}
		else if(RIGHT_BRACKET == item) {
			--level;

		}
		else if(AND_STR == upper(item)) {
			logic = LOGIC_AND;

		}
		else if(OR_STR == upper(item)) {
			logic = LOGIC_OR;

		}
		else if(is_operation(upper(item))) {
			operation = item;

		}
		else if(is_op1) {
			if(LEFT_BRACKET_CHAR == item.at(0)) {
				operator1 = ltrim(item, LEFT_BRACKET);
				++level;
				logic = LOGIC_AND;

			}

			is_op1 = false;
		}
		else {
			if(RIGHT_BRACKET_CHAR == item.at(item.size() - 1)) {
				operator2 = rtrim(item, RIGHT_BRACKET);
			}

			condition c = condition();
			c.logic = logic;
			c.level = level;
			c.operator1 = operator1;
			c.operation = operation;
			c.operator2 = operator2;
			where.push_back(c);

			is_op1 = true;

			if(RIGHT_BRACKET_CHAR == item.at(item.size() - 1)) {
				--level;

			}
		}

		++it;
	}

	return where;
}

bool is_operation(const string &str) {
	return str == EQUAL_STR
		|| str == NOT_EQUAL_STR
		|| str == GT_STR
		|| str == GTE_STR
		|| str == LT_STR
		|| str == LTE_STR;
}

vector<string> get_group(vector<string> groups) {
	// 去除首位的BY
	map<string, unsigned int> group_by = map<string, unsigned int>();
	vector<string> new_groups(groups.begin() + 1, groups.end());

	return split(join(new_groups, BLANK_STR), COMMA_STR);

}

vector<order> get_order(vector<string> orders) {
	vector<order> order_by = vector<order>();
	vector<string> new_orders(orders.begin() + 1, orders.end());

	new_orders = split(join(new_orders, SPACE_STR), COMMA_STR);


	vector<string>::iterator it = new_orders.begin();

	while(new_orders.end() != it) {
		string item_raw = trim(*it, SPACE_STR);

		vector<string> items = split(item_raw, SPACE_STR);
		order o = order();

		if(1 == items.size()) {
			o.name = items.at(0);
			o.type = ORDER_ASC;
			order_by.push_back(o);

		}
		else if(2 == items.size()) {
			o.name = items.at(0);
			o.type = ASC_STR == items.at(1);
			order_by.push_back(o);
		}

		++it;
	}

	return order_by;
}

void print_sql(const sql &query) {
	cout << "SQL structure:" << endl;
	// 输出SELECT
	cout << "SELECT:" << endl;
	vector<field>::const_iterator select_it = query.select.begin();
	while(query.select.end() != select_it) {
		cout << "\tname: " << select_it->name << endl;
		cout << "\ttype: " << select_it->type << endl;

		++select_it;
	}

	// 输出FROM
	cout << "FROM:" << endl;
	cout << "\tFROM_TYPE: " << query.from_type << " FROM: " << query.from << endl;

	// 输出WHERE
	vector<condition>::const_iterator where_it = query.where.begin();
	while(query.where.end() != where_it) {
		cout << "\tlogic: " << where_it->logic << endl;
		cout << "\tlevel: " << where_it->level << endl;
		cout << "\toperator1: " << where_it->operator1 << endl;
		cout << "\toperation: " << where_it->operation << endl;
		cout << "\toperator2: " << where_it->operator2 << endl;

		++where_it;
	}

	// 输出GROUP BY
	cout << "GROUP BY:" << endl;
	vector<string>::const_iterator group_it = query.group_by.begin();
	while(query.group_by.end() != group_it) {
		cout << "\t" << *group_it << endl;

		++group_it;
	}

	// 输出ORDER BY
	cout << "ORDET BY:" << endl;
	vector<order>::const_iterator order_it = query.order_by.begin();
	while(query.order_by.end() != order_it) {
		cout << "\tname: " << order_it->name << endl;
		cout << "\ttype: " << order_it->type << endl;

		++order_it;
	}

}
