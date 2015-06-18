/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     Mon 15 Jun 2015 08:19:50 PM CST
*  File:     q++.cpp
*  Desc:     main function of this project
*/

#include <vector>
#include <string>
#include <iostream>

#include "sql.h"
#include "util.h"

using namespace std;

void usage(void) {
	cerr << "Usage:" << endl;
	cerr << "	q++ 'select count(*) from example.csv where id > 100'" << endl;
}


int main(int argc, char **argv) {
	// query example
	// q++ 'select count(*) from xxx.csv where id > 10'
	if(argc < 2) {
		usage();
		return 1;
	}

	bool with_header = false;
	string delimeter;
	for(int i = 0; i < argc; ++i) {
		if("--with-header" == string(argv[i])) {
			with_header = true;
		}
		else if("--delimeter" == string(argv[i]) || "-d" == string(argv[i])) {
			if(i + 1 < argc) {
				delimeter = argv[i++];

			}
			else {
				cerr << argv[i] << " option requires one argument." << endl;
				return 1;
			}

		}

	}
	char *sqlstr = argv[1];
	cout << sqlstr << endl;

	//string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') group by id, name order by id, name asc");
	//string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') group by id, name");
	string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') order by id, name asc");
	cout << query << endl;
	sql result = sql();
	sql_init(query, result);


	return 0;
}
