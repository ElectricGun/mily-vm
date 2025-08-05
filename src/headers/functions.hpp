#pragma once

#include <map>
#include <string>
#include <vector>

using namespace std;

namespace mily {
    
    struct Node {
        int value = -1;
    };

    struct JumpTarget {
        int line_number = 0;
        int delete_offset = 0;
    };

    bool is_numeric(string line);

    vector<string> load_file(string file_name);

    map<string, JumpTarget> make_jump_table(vector<string>& code);

    vector<string> prepare_code(vector<string> &code, map<string, JumpTarget> &jump_table);

    vector<string> prepare_code(vector<string>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels);
}