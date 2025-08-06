#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.hpp"

using namespace std;

namespace mily {
    
    struct GlobalVar {
        string value = "";
    };

    struct ActiveVar {
        string type = TYPE_DOUBLE;
        double double_value = 0;
        bool is_null = true;
    };

    bool compare(ActiveVar left, string comparator, ActiveVar right);

    ActiveVar get_active_var(int& counter, string& token, map<string, GlobalVar>& active_var_map);

    void operate(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map);

    void jump(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map);

    void set(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map);

    void print_buffer(int& counter, vector<string>& line, string& printbuffer, map<string, GlobalVar>& active_var_map);

    void forward(int& counter, int max_lines);
}