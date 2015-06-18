/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     Mon 15 Jun 2015 08:19:50 PM CST
*  File:     q++.cpp
*  Desc:     main function of this project
*/

#include <vector>
#include <string>

#include "sql.h"
#include "util.h"
#include "iostream"

using namespace std;

void usage(void) {
	cerr << "Usage:" << endl;
	cerr << "	q++ 'select count(*) from example.csv where id > 100'" << endl;
}


int main(int n, char **args) {
	// query example
	// q++ 'select count(*) from xxx.csv where id > 10'
	if(2 != n) {
		usage();
		return 0;
	}
	char *sqlstr = args[1];
	cout << sqlstr << endl;

	string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100");
	cout << replace_all(query, "select", "test") << endl;
	sql result = sql();
	sql_init(query, result);


	return 0;
}
