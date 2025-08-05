#include <map>
#include <vector>
#include <sstream>
#include <iostream>
#include "headers/functions.hpp"
#include "headers/constants.hpp"

using namespace std;
using namespace mily;

namespace mily {

    bool is_numeric(string line) {
        char* p;
        strtol(line.c_str(), &p, 10);
        return *p == 0;
    }

    vector<string> load_file(string file_name) {
        ifstream file(file_name);
        vector<string> output;
        
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                if (!line.empty()) {
                    output.push_back(line);
                }
            }    
        } else {
            cerr << "Error opening file \"" << file_name << "\"";
        }

        return output;
    }

    map<string, JumpTarget> make_jump_table(vector<string>& code) {
        int line_number = 0;
        int jump_offset = 0;
        map<string, JumpTarget> jump_map;

        for (string& line : code) {
            stringstream line_stream(line);

            string word;

            bool found_label = false;
            bool is_label_one_liner = false;

            while (line_stream >> word) {
                if (word.back() == ':') {
                    found_label = true;
                    is_label_one_liner = true;

                    jump_map.insert({
                        word.substr(0, word.size() - 1), 
                        JumpTarget{line_number, jump_offset}
                    });

                } else if (found_label) {
                    is_label_one_liner = false;
                }
            }
            if (found_label && is_label_one_liner) {
                jump_offset--;
            }
            line_number++;
        }

        return jump_map;
    }

    vector<string> prepare_code(vector<string> &code, map<string, JumpTarget> &jump_table) {
        return prepare_code(code, jump_table, true);
    }

    vector<string> prepare_code(vector<string>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels) {
        vector<string> new_code;

        int line_number = 0;
        for (string& line : code) {
            stringstream line_stream(line);
            string new_line = "";
            
            bool is_jump = false;
            int word_number = 0;
            string word;
            while (line_stream >> word) {
                if (word.back() == ':') {
                    if (!do_delete_labels) {
                        new_line.append(word);
                    }
                } else {
                    if (word == KEY_JUMP) {
                        is_jump = true;
                        new_line.append(word + " ");

                    } else if (word_number == 1 && is_jump) {
                        new_line.append(to_string(jump_table[word].line_number + (do_delete_labels ? jump_table[word].delete_offset : 0)) + " ");
                    
                    } else {
                        new_line.append(word + " ");
                    }
                }
                word_number++;
            }
            if (!new_line.empty()) {
                new_code.push_back(new_line);
            }
            line_number++;
        }

        return new_code;
    }
}