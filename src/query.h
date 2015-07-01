/*
*  Author:   huangjunwei@youmi.net
*  	     suyuhui@youmi.net
*  Time:     Tue 18 Jun 2015 14:43:18 PM CST
*  File:     query.h
*  Desc:
*/

#ifndef _QUERY_H_
#define _QUERY_H_

#include <string>
#include <map>
#include "sql.h"

using namespace std;


void line_iterator(istream &input, const bool &with_header, const string &delimeter);



//map<string,int> header_map;

//bool where(const string &line,const string &seperator,const sql &source);

/*
 *simple select column
 */
//string select(const string &line,const string &seperator,const sql &source);

/*
 *a aggregate function like count,sum,max,min,avg followed by select
 */
//void select_with_group(const string &line,const string &seperator,map<string,string> &result,const sql &source,unsigned int &number_record);

//void start(const sql &source,bool has_header,const string &seperator);
//void input_stream(istream &is,bool has_header,const string &seperator,const sql &source);
#endif
