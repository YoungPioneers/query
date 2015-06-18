/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     Tue 16 Jun 2015 11:24:21 AM CST
*  File:     sql.h
*  Desc:
*/

#ifndef _SQL_H_
#define _SQL_H_

#include <string>
#include <vector>
#include <map>

using namespace std;

// TODO 方差: var, varp, 标准差: stdev, stdevp
const unsigned int SELECT_PLAIN 	= 0x00;
const unsigned int SELECT_SUM 		= 0x01;
const unsigned int SELECT_MAX 		= 0x02;
const unsigned int SELECT_MIN 		= 0x04;
const unsigned int SELECT_AVG 		= 0x08;
const unsigned int SELECT_COUNT 	= 0x10;
const unsigned int SELECT_DISTINCT 	= 0x20;

// 组合查询类型
const unsigned int SELECT_COUNT_DISTINCT = SELECT_COUNT & SELECT_DISTINCT;

const string SELECT_STR 		= "SELECT";
const string FROM_STR 			= "FROM";
const string WHERE_STR 			= "WHERE";
const string GROUP_STR 			= "GROUP";
const string ORDER_STR 			= "ORDER";
const string AND_STR 			= "AND";
const string OR_STR 			= "OR";
const string SUM_STR 			= "SUM";
const string MAX_STR 			= "MAX";
const string MIN_STR 			= "MIN";
const string AVG_STR 			= "AVG";
const string COUNT_STR 			= "COUNT";
const string DISTINCT_STR 		= "DISTINCT";
const string BY_STR 			= "BY";
const string ASC_STR 			= "ASC";
const string DESC_STR 			= "DESC";

const bool FROM_FILE 			= false;
const bool FROM_STDIN 			= true;
const string FROM_STDIN_STR		= "-";

const bool LOGIC_AND 			= true;
const bool LOGIC_OR 			= false;

const string EQUAL_STR 			= "=";
const string NOT_EQUAL_STR 		= "<>";
const string GT_STR 			= ">";
const string GTE_STR 			= ">=";
const string LT_STR 			= "<";
const string LTE_STR 			= "<=";
const string LEFT_BRACKET  		= "(";
const string RIGHT_BRACKET  		= ")";
const char LEFT_BRACKET_CHAR 		= '(';
const char RIGHT_BRACKET_CHAR 		= ')';
const string COMMA_STR 			= ",";

const bool ORDER_ASC 			= true;
const bool ORDER_DESC  			= false;

using namespace std;

// single where condition struct
typedef struct condition_s {
	// true for and, false for or
	// xxxx || (logic && condiction result)
	bool logic = LOGIC_AND;
	unsigned int level = 0;

	string operator1, operator2, operation;

} condition, *conditionptr;

typedef struct sql_s {
	// select clause
	map<string, unsigned int> select;

	// from clause
	bool from_type = FROM_FILE;
	string from;

	// where clause
	vector<condition> where;

	// group by clause
	vector<string> group_by;

	// order by clause
	map<string, bool> order_by;

} sql, *sqlptr;

// 初始化sql struct并作语法检测
bool sql_init(const string &query, sql &result);

// 查找select, from, where, group, order等关键字位置
void find_keyword_pos(vector<string> items, size_t &select_pos, size_t &from_pos, size_t &where_pos, size_t &group_pos, size_t &order_pos);

// 通过关键字位置判断语法正确性
bool syntax_check(const size_t &size, const size_t &select_pos, const size_t &from_pos, const size_t &where_pos, const size_t &group_pos, const size_t &order_pos);

// 处理 select 的 fields
map<string, unsigned int> get_fields(vector<string> fields);

// 处理 where 条件字段
vector<condition> get_where(vector<string> conditions);

// 判断字符串是否为操作符
bool is_operation(const string &str);

// 处理group by语句
vector<string> get_group(vector<string> groups);

// 处理order by 语句
map<string, bool> get_order(vector<string> orders);
#endif
