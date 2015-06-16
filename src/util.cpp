/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     二  6/16 22:09:46 2015
*  File:     util.cpp
*  Desc:   
*/

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

using namespace std;

vector<string> split(const string &str, const string &delimeter) {
	size_t it = 0;
	string src = str;
	vector<string> items = vector<string>();

	while(string::npos != it) {
		it = src.find(delimeter, 0);
		items.push_back(src.substr(0, it));
		src = src.substr(it + 1, src.size());
	}
	return items;
}
