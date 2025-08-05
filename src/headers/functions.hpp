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

    struct Line {
        string value;
        int line;
    };

    bool is_numeric(string line);

    vector<Line> load_file(string file_name);

    map<string, JumpTarget> make_jump_table(vector<Line>& code);

    vector<Line> prepare_code(vector<Line> &code, map<string, JumpTarget> &jump_table);

    vector<Line> prepare_code(vector<Line>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels);
}