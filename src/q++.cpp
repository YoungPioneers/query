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
	cerr << "Usage: q++ [options] query_string" << endl;
	cerr << "	--delimeter	String used to seperate fields; Default set to a space; -d for short." << endl;
	cerr << "	--help		To display this page. -h for short." << endl;
	cerr << "	--with-header	Must add this option if the first row of input file is a line of field names." << endl;
	cerr << endl;
	cerr << "Example:" << endl;
	cerr << "	q++ 'select sum(fee) from example.csv where id > 100'" << endl;
	cerr << "	cat example.csv | q++ 'select sum(fee) from - where id > 100'" << endl;
}


int main(int argc, char **argv) {
	bool with_header = false;
	string delimeter(" "), query("");

	// query example
	// q++ 'select count(*) from xxx.csv where id > 10'
	if(argc < 2) {
		usage();
		return 1;
	}

	for(int i = 0; i < argc; ++i) {
		string argstr = string(argv[i]);
		if("--help" == argstr || "-h" == argstr) {
			usage();
			return 1;
		}

		else if("--with-header" == argstr) {
			with_header = true;
		}

		else if("--delimeter" == argstr || "-d" == argstr) {
			if(i + 1 < argc) {
				delimeter = argv[i++];

			}
			else {
				cerr << argv[i] << " option requires one argument." << endl;
				return 1;
			}

		}

		else {
			query = argstr;
			
		}

	}

	if(string("") == query) {
		usage();
		return 1;
	}
	//string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') group by id, name order by id, name asc");
	//string query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') group by id, name");
	query = string("select count(distinct(id)), name from example.csv where id > 10 and id < 100 or (name = 'eddie' or name = 'yuhui') order by id, name asc");
	cout << query << endl;
	sql result = sql();
	sql_init(query, result);


	return 0;
}
