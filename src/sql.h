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

// TODO 方差: var, varp, 标准差: stdev, stdevp
#define SELECT_PLAIN 		0x00 	// select column directly
#define SELECT_SUM 		0x01	// workout sum of the column
#define SELECT_MAX 		0x02
#define SELECT_MIN 		0x04
#define SELECT_AVG 		0x08
#define SELECT_COUNT 		0x10
#define SELECT_DISTINCT 	0x20

// 组合查询类型
#define SELECT_COUNT_DISTINCT (SELECT_COUNT & SELECT_DISTINCT)

#define SELECT_STR 		"SELECT"
#define FROM_STR 		"FROM"
#define WHERE_STR 		"WHERE"
#define GROUP_STR 		"GROUP"
#define ORDER_STR 		"ORDER"
#define AND_STR 		"AND"
#define OR_STR 			"OR"
#define SUM_STR 		"SUM"
#define MAX_STR 		"MAX"
#define MIN_STR 		"MIN"
#define AVG_STR 		"AVG"
#define COUNT_STR 		"COUNT"
#define DISTINCT_STR 		"DISTINCT"
#define BY_STR 			"BY"
#define ASC_STR 		"ASC"
#define DESC_STR 		"DESC"

#define FROM_FILE 		false
#define FROM_STDIN 		true
#define FROM_STDIN_STR		"-"

#define LOGIC_AND		true
#define LOGIC_OR		false

#define EQUAL_STR		"="
#define NOT_EQUAL_STR		"<>"
#define GT_STR			">"
#define GTE_STR			">="
#define LT_STR			"<"
#define LTE_STR			"<="
#define LEFT_BRACKET 		"("
#define RIGHT_BRACKET 		")"
#define LEFT_BRACKET_CHAR 	'('
#define RIGHT_BRACKET_CHAR 	')'
#define COMMA_STR		","

#define ORDER_ASC		true
#define ORDER_DESC 		false

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
	//map<string, unsigned int> group_by;

	// order by clause
	map<string, bool> order_by;

} sql, *sqlptr;

// 初始化sql struct并作语法检测
bool sql_init(const string &query, sql &result);

// 查找select, from, where, group, order等关键字位置
bool find_keyword_pos(vector<string> items, size_t &select_pos, size_t &from_pos, size_t &where_pos, size_t &group_pos, size_t &order_pos);

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
//map<string, unsigned int> get_group(vector<string> groups);

// 处理order by 语句
map<string, bool> get_order(vector<string> orders);
#endif
