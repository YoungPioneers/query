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

#include "sql.h"
#include "util.h"

using namespace std;

// example select id from example.csv where id > 10 and id < 100;
void sql_init(const string &query, sql &result) {
	vector<string> items = string_split(query, string(" "));
	cout << query << endl;
}
