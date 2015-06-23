#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "query.h"
#include "util.h"

using namespace std;

map<string,int> header_map = map<string,int>();

void input_stream(istream &is,bool has_header,const string &seperator,const sql &source) {
    string line;
    getline(is,line);
    const string const_header_line = line;
    vector<string> header_vector = split(const_header_line,seperator);
    int index = 1;

    for(vector<string>::iterator it = header_vector.begin();it != header_vector.end();++it) {
        if(has_header){
            header_map[*it] = index;
        }else{
            header_map["$" + to_string(index)] = index;
        }

        ++index;
    }

    while(getline(is,line)) {
        const string const_line = line;
        if(source.where.size() !=0 && !where(const_line,seperator,source)) {
            continue;
        }

    }
}

void start(const sql &source,bool has_header,const string &seperator) {
    if (source.from_type) {
        input_stream(cin,has_header,seperator,source);
    } else {
        ifstream infile(source.from);
        input_stream(infile,has_header,seperator,source);
        infile.close();
    }
}

string select(const string &line,const string seperator,const sql &source) {
    vector<string> fields = split(line,seperator);
    string result = "";
    for(vector<const field>::iterator it = source.select.begin();it != source.select.end();++it) {

        if(it->type != SELECT_PLAIN) {
            continue;
        }

        if(header_map.find(it->name) == header_map.end()) {
            cout << it->name + " not found in header_map" << endl;
            return "";
        }

        int index = header_map[it->name];
        result += fields[index] + seperator;
    }

    int length = result.length();
    result = length > 0 ? result.substr(0,length - seperator.length()):"";
    return result;
}

void select_with_group(const string &line,const string &seperator,map<string,map<unsigned int,float>> &result,const sql &source,unsigned int &number_record) {

    vector<string> fields = split(line,seperator);

    string key = "";
    for(vector<const string>::iterator it = source.group_by.begin();it != source.group_by.end();++it) {
        if(header_map.find(*it) == header_map.end()) {
            cout << *it + "not found in header_map" << endl;
            return;
        }

        int index = header_map[*it];
        string field_value = fields[index];
        key += field_value + seperator;
    }

    map<string,map<unsigned,float>>::iterator it_exist = result.find(key);
    if( it_exist == result.end()) {
        result[key] = map<unsigned int,float>();
    }

    for(vector<const field>::iterator it = source.select.begin(); it != source.select.end(); ++it) {
        if(it->type != SELECT_PLAIN) {
            switch(it->type){
                case SELECT_SUM:{
                        float current_sum = result[key][SELECT_SUM];
                        float add = stof(fields[header_map[it->name]]);
                        current_sum += add;
                        result[key][SELECT_SUM] = current_sum;
                        break;
                    }
                case SELECT_MAX:{
                        float current_max = result[key][SELECT_MAX];
                        float current_value_max = stof(fields[header_map[it->name]]);
                        result[key][SELECT_MAX] = current_max > current_value_max ? current_max:current_value_max;
                        break;
                    }
                case SELECT_MIN:{
                        float current_min = result[key][SELECT_MIN];
                        float current_value_min = stof(fields[header_map[it->name]]);
                        result[key][SELECT_MIN] = current_min > current_value_min ? current_value_min:current_min;
                        break;
                    }
                case SELECT_AVG:{
                        float current_avg = result[key][SELECT_AVG];
                        float current_value = stof(fields[header_map[it->name]]);
                        float total =  (current_avg * number_record + current_value );
                        number_record ++;
                        result[key][SELECT_AVG] = total / number_record;
                        break;
                    }
                case SELECT_COUNT:{
                        float current_count = result[key][SELECT_COUNT];
                        float current_value_count = stof(fields[header_map[it->name]]);
                        result[key][SELECT_COUNT] = current_count + current_value_count;
                        break;
                    }
                case SELECT_DISTINCT:
                   //这里会有哪些情形？这里不太合理
                    result[it->name] = map<unsigned int,float>();
                    break;
                default:
                    char type[16];
                    sprintf(type, "%d", it->type);
                    string type_str = type;
                    cout << type_str + " aggr not found" << endl;
            }
        }
    }
}

bool where(const string &line,const string &seperator,const sql &source) {

    vector<string> fields = split(line,seperator);
    bool last_step_pass = false;
    for(vector<const condition>::iterator it = source.where.begin(); it != source.where.end();++it) {

        if(last_step_pass && !it->logic) {
            continue;
        }

        string op1 = it->operator1;
        string op2 = it->operator2;
        string oper = it->operation;

        if(header_map.find(op1) == header_map.end()) {
            continue;
        }

        int index = header_map[op1];
        string field = fields[index-1];
        int compare_result = field.compare(op2);
        bool current_step_pass = false;
        if(oper == EQUAL_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else if(oper == NOT_EQUAL_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else if(oper == GT_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else if(oper == GTE_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else if(oper == LT_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else if(oper == LTE_STR) {
            current_step_pass = compare_result != 0 ? false:true;
        }else {
            last_step_pass = false;
            continue;
        }

        if(!current_step_pass) {
            last_step_pass = false;
            continue;
        }

        last_step_pass = it->logic && !last_step_pass ? false:true;
    }

    return last_step_pass;
}

int main() {
    return 0;
}
