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
#define SELECT_PLAIN 0x00	// select column directly
#define SELCT_SUM 0x01		// workout sum of the column
#define SELCT_MAX 0x02
#define SELCT_MIN 0x04
#define SELCT_AVG 0x08
#define SELCT_COUNT 0x10
#define SELCT_DISTINCT 0x20

// 组合查询类型
#define SELECT_COUNT_DISTINCT (SELCT_COUNT & SELCT_DISTINCT)

#define FROM_FILE 0x01
#define FROM_STDIN 0x02

using namespace std;

const string SELECT_STR = string("SELECT");
const string FROM_STR = string("FROM");
const string WHERE_STR = string("WHERE");
const string GROUP_STR = string("GROUP");
const string ORDER_STR = string("ORDER");
const string AND_STR = string("AND");
const string OR_STR = string("OR");

// single where condition struct
typedef struct condition_s {
	// true for and, false for or
	// xxxx || (logic && condiction result)
	bool logic:true;

	string operator1, operator2, operation;

} condition, *conditionptr;

typedef struct sql_s {
	// select clause
	map<string, int> select;

	// from clause
	bool from_file:true;
	string from_string;

	// where clause
	vector<condition> where;

	// group by clause
	vector<string> group_by;

	// order by clause
	vector<string> order_by;

} sql, *sqlptr;

bool sql_init(const string &query, sql &result);

// 查找select, from, where, group, order等关键字位置
bool find_keyword_pos(vector<string> items, size_t &select_pos, size_t &from_pos, size_t &where_pos, size_t &group_pos, size_t &order_pos);

// 通过关键字位置判断语法正确性
bool syntax_check(const size_t &size, const size_t &select_pos, const size_t &from_pos, const size_t &where_pos, const size_t &group_pos, const size_t &order_pos);

// 处理 select 的 fields
map<string, int> get_fields(vector<string> fields);

#endif
