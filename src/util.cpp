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

vector<string> split(const string &str, const string &delimeter){
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

string upper(const string &str) {
	string t(str);
	transform(t.begin(), t.end(), t.begin(), ::toupper);
	return t;
}

string lower(const string &str) {
	string t(str);
	transform(t.begin(), t.end(), t.begin(), ::tolower);
	return t;
}

string reverse(const string &str) {
	string t(str.rbegin(), str.rend());
	return t;
}

string ltrim(const string &str, const string &s = " ") {
	size_t pos = str.find_first_not_of(s);
	return ( pos == string::npos ) ? "" : str.substr(pos);
}

string rtrim(const string &str, const string &s = " ") {
	size_t pos = str.find_last_not_of(s);
	return ( pos == string::npos ) ? "" : str.substr(0, pos + 1);
}

string trim(const string &str, const string &s = " ") {
	return rtrim(ltrim(str, s), s);
}

string join(vector<string> &strs, const string &seperator) {
	string t("");
	vector<string>::iterator it = strs.begin();
	t += *it;
	while(strs.end() != ++it) {
		t += seperator + *it;
	}
	return t;

}
