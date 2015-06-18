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

#include "sql.h"
#include "util.h"

using namespace std;

// 仅支持简单select
// example select id from example.csv where id > 10 and id < 100;
bool sql_init(const string &query, sql &result) {
	vector<string> items = split(query, string(" "));


	size_t size = items.size(), pos = 0;
	size_t select_pos(size), from_pos(size), where_pos(size), group_pos(size), order_pos(size);

	if(!find_keyword_pos(items, select_pos, from_pos, where_pos, group_pos, order_pos)) {
		return false;
	}

	vector<string> fields(items.begin() + select_pos + 1, items.begin() + from_pos);
	vector<string> table(items.begin() + from_pos + 1, items.begin() + where_pos);
	vector<string> conditions(items.begin() + where_pos + 1, items.begin() + group_pos);

	// 处理fields
	fields = split(join(fields, ""), ",");
	result.select = get_fields(fields);

	return true;
}

bool find_keyword_pos(vector<string> items, size_t &select_pos, size_t &from_pos, size_t &where_pos, size_t &group_pos, size_t &order_pos) {
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
	return syntax_check(size, select_pos, from_pos, where_pos, group_pos, order_pos);
}

bool syntax_check(const size_t &size, const size_t &select_pos, const size_t &from_pos, const size_t &where_pos, const size_t &group_pos, const size_t &order_pos) {
	return select_pos < size && from_pos < size && select_pos < from_pos;
}

map<string, unsigned int> get_fields(vector<string> fields) {
	map<string, unsigned int> select = map<string, unsigned int>();

	vector<string>::iterator it = fields.begin();
	while(fields.end() != it) {
		// 提取最外层括号
		// count (distince(id)) / name
		string field_raw = *it;

		// 解析嵌套的函数调用
		size_t left, right;
		unsigned int function_type = SELECT_PLAIN;
		string function_name(""), field_name("");
		while(true) {
			left = field_raw.find("(", 0);
			right = field_raw.rfind(")", field_raw.size());
			//cout << field_raw << " " << left << " " << right << endl;

			if(string::npos == left || string::npos == right) {
				// 函数最里层
				field_name = field_raw;
				//cout << "field_name: " << field_name << endl;
				//cout << "function_type: " << function_type << endl;

				select[field_name] = function_type;
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

			//cout << "function_name: " << function_name << endl;
			//cout << "field_name: " << field_name << endl;
			//cout << "function_type: " << function_type << endl;

			field_raw = field_name;


		}


		++it;
	}
	return select;
}
