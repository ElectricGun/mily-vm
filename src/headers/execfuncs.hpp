#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.hpp"

using namespace std;

namespace mily {

    struct ActiveVar {
        int type = TYPE_NULL;
        double double_value = 0;
    };

    bool compare(ActiveVar& left, string& comparator, ActiveVar& right);

    ActiveVar parse_to_active_var(int& counter, string& token, map<string, ActiveVar>& active_var_map);

    void operate(int& counter, vector<string>& line, map<string, ActiveVar>& active_var_map);

    void jump(int& counter, vector<string>& line, map<string, ActiveVar>& active_var_map);

    void set(int& counter, vector<string>& line, map<string, ActiveVar>& active_var_map);

    void print_buffer(int& counter, vector<string>& line, string& printbuffer, map<string, ActiveVar>& active_var_map);

    void forward(int& counter, int& max_lines);
}