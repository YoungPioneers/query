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

// single where condition struct
typedef struct condition_s {
	// true for and, false for or
	// xxxx || (logic && condiction result)
	bool logic:true;

	string operator1, operator2, operation;

	//condition_s(string operator1, string operator2, string operation);
	//~condition_s();

} condition, *conditionptr;

typedef struct sql_s {
	// select clause
	map<string, int> select;

	// from clause
	bool from_file:true;
	string from_string;

	// where clasuse
	vector<condition> where;

} sql, *sqlptr;

void sql_init(const string &query, sql &result);

#endif
