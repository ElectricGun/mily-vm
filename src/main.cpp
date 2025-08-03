#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include "headers/functions.h"
#include "headers/executor.h"

using namespace std;
using namespace functions;
using namespace executor;

static const vector<string> single_flags {
    "-v"
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
                exit (EXIT_FAILURE);

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
            exit (EXIT_FAILURE);
        }
    }

    if (flag_mode) {
        cerr << "Missing argument value after flag" << endl;
        exit (EXIT_FAILURE);
    }

    // process arguments
    bool verbose = false;
    for (auto& x : flags) {
        cout << x.first << "  " << x.second << endl;
        
        if (x.first == "-v") {
            if (x.second == "0") {
                verbose = false;
            
            } else if (x.second == "1") {
                verbose = true;

            } else {
                cerr << "Unexpected value after flag \"" << x.first << "\"" << endl;
                exit(EXIT_FAILURE);
            }
        }
    }

    vector<string> code = load_file(file_name);
    map<string, JumpTarget> jump_table = make_jump_table(code);
    vector<string> code_processed = prepare_code(code, jump_table);

    execute(code_processed, verbose);

    return 0;
}