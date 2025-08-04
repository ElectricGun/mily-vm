#pragma once

#include <string>
#include "constants.h"

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

    ActiveVar get_active_var(string& token, map<string, GlobalVar>& active_var_map);

    void operate(string& line, map<string, GlobalVar>& active_var_map);

    void jump(int& counter, string& line, map<string, GlobalVar>& active_var_map);

    void set(string& line, map<string, GlobalVar>& active_var_map);
}