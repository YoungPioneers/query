/*
*  Author:   suyuhui@youmi.net
*  	     huangjunwei@youmi.net
*  Time:     二  6/16 22:15:03 2015
*  File:     util.h
*  Desc:
*/

#ifndef _UTIL_H_
#define _UTIL_H_

#include <vector>
#include <string>

using namespace std;

// 字符串分割 delimeter -> 分隔符
vector<string> split(const string &str, const string &delimeter);

// 大小写转换
string upper(const string &str);
string lower(const string &str);

// 字符串反转
string reverse(const string &str);

// 字符串trim
string ltrim(const string &str, const string &s);
string rtrim(const string &str, const string &s);
string trim(const string &str, const string &s);

// 字符串数组->字符串
string join(vector<string> &strs, const string &seperator);

#endif
