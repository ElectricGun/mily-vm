#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.hpp"

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

    struct ActiveVar {
        int type = TYPE_NULL;
        int int_value;
        double double_value;
    };

    struct Token {
        int type = TYPE_NULL;
        union {
            int int_value;
            double double_value;
        };
        string string_value;
    };

    struct Instruction {
        int line;
        int id;
        vector<Token> content;
    };

    bool is_numeric(string line);

    vector<Line> load_file(string file_name);

    map<string, JumpTarget> make_jump_table(vector<Line>& code);

    vector<Line> prepare_code(vector<Line> &code, map<string, JumpTarget> &jump_table);

    vector<Line> prepare_code(vector<Line>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels);

    vector<Instruction> prepare_code_instructions(vector<Line>& code);
}