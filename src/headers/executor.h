#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.h"

using namespace std;

namespace executor {
    struct GlobalVar {
        string value = "";
    };

    struct ActiveVar {
        string type = TYPE_DOUBLE;
        double double_value = 0;
        bool is_null = true;
    };

    void execute(vector<string> code, bool verbose);
}