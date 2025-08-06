#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "headers/functions.hpp"
#include "headers/executor.hpp"

using namespace std;
using namespace mily;

// TODO: make arg parsing better

static const vector<string> single_flags {
    "-v", "-bm", "--benchmark", "--limit"
};

bool is_flag(string arg) {
    return arg.front() == '-';
}

int main(int argc, char* argv[]) {
    string file_name;

    // store arguments
    map<string, string> flags;
    string current_flag;
    bool flag_mode = false;
    bool file_name_set = false;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (is_flag(arg)) {
            if (flag_mode) {
                cerr << "Invalid argument \"" << arg << "\"";
                exit(EXIT_FAILURE);

            } else {
                if (count(single_flags.begin(), single_flags.end(), arg)) {
                    flags[arg] = "1";

                } else {
                    flag_mode = true;
                    current_flag = arg;
                }
            }
        } else if (flag_mode) {
            flags[current_flag] = arg;
            flag_mode = false;
        
        } else if (!flag_mode) {
            file_name = arg;
            file_name_set = true;
            
        } else {
            cerr << "Invalid argument \"" << arg << "\"";
            exit(EXIT_FAILURE);
        }
    }

    if (flag_mode) {
        cerr << "Missing argument value after flag" << endl;
        exit(EXIT_FAILURE);
    }

    // process arguments
    bool verbose = false;
    bool benchmark = false;
    bool limit = false;
    for (auto& x : flags) {        
        // TODO: this is inefficient
        if (x.first == "-v") {
            if (x.second == "0") {
                verbose = false;
            
            } else if (x.second == "1") {
                verbose = true;

            } else {
                cerr << "Unexpected value after flag \"" << x.first << "\"" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (x.first == "-bm" || x.first == "--benchmark") {
            if (x.second == "0") {
                benchmark = false;
            
            } else if (x.second == "1") {
                benchmark = true;

            } else {
                cerr << "Unexpected value after flag \"" << x.first << "\"" << endl;
                exit(EXIT_FAILURE);
            }
        } else if (x.first == "--limit") {
            if (x.second == "0") {
                limit = false;
            
            } else if (x.second == "1") {
                limit = true;

            } else {
                cerr << "Unexpected value after flag \"" << x.first << "\"" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    vector<Line> code = load_file(file_name);
    map<string, JumpTarget> jump_table = make_jump_table(code);
    // for (auto l : jump_table) {
    //     cout << l.first << " -> " << l.second.line_number << " + " << l.second.delete_offset << endl;
    // }
    vector<Line> code_processed = prepare_code(code, jump_table);
    vector<Instruction> instructions = prepare_code_instructions(code_processed);

    // for (Instruction ins : instructions) {
    //     string out;
    //     for (string s : ins.content) {
    //         out.append(s).append(" ");
    //     }
    //     cout << ins.line << ": " << ins.line << " " << out << endl;
    // }

    execute(instructions, verbose, benchmark, limit);

    return 0;
}