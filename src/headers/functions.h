#pragma once

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

namespace functions {
    struct Node {
        int value = -1;
    };

    struct JumpTarget {
        int line_number = 0;
        int delete_offset = 0;
    };

    vector<string> load_file(string file_name);

    map<string, JumpTarget> make_jump_table(vector<string>& code);

    vector<string> prepare_code(vector<string> &code, map<string, JumpTarget> &jump_table);

    vector<string> prepare_code(vector<string>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels);
}