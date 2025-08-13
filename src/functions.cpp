#include <cstdlib>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "headers/functions.hpp"
#include "headers/constants.hpp"
#include "headers/maps.hpp"

using namespace std;
using namespace mily;

namespace mily {

    bool is_numeric(string line) {
        char* p;
        strtod(line.c_str(), &p);
        return *p == 0;
    }

    vector<Line> load_file(string file_name) {
        ifstream file(file_name);
        vector<Line> output;
        
        if (file.is_open()) {

            int line_number = 0;
            string line;
            while (getline(file, line)) {
                bool comment_mode = false;
                string new_line;
                stringstream line_stream(line);
                if (!line.empty()) {
                    string word;
                    while (line_stream >> word) {
                        if (!word.compare(0, KEY_COMMENT.size(), KEY_COMMENT)) {
                            comment_mode = true;
                            continue;
                        
                        } else if (!comment_mode) {
                            new_line.append(word + " ");
                        } 
                    }
                    if (new_line.length() > 0) {
                        // cout << new_line << endl;
                        struct Line line_struct{new_line, line_number};
                        output.push_back(line_struct);
                    }
                }
                line_number ++;
            }    
        } else {
            cerr << "Error opening file \"" << file_name << "\"" << endl;
            exit(EXIT_FAILURE);
        }

        return output;
    }

    map<string, JumpTarget> make_jump_table(vector<Line>& code) {
        int line_number = 0;
        int jump_offset = 0;
        map<string, JumpTarget> jump_map;

        for (Line& line_struct : code) {
            string line = line_struct.value;
            stringstream line_stream(line);

            string word;

            bool found_label = false;
            bool is_label_one_liner = false;
            bool not_label = false;

            while (line_stream >> word) {
                if (word.back() == ':' && !not_label) {
                    found_label = true;
                    is_label_one_liner = true;

                    jump_map.insert({
                        word.substr(0, word.size() - 1), 
                        JumpTarget{line_number, jump_offset}
                    });

                } else {
                    not_label = true;
                    if (found_label) {
                    is_label_one_liner = false;
                    }
                }
            }
            if (found_label && is_label_one_liner) {
                jump_offset--;
            }
            line_number++;
        }

        return jump_map;
    }

    vector<Line> prepare_code(vector<Line> &code, map<string, JumpTarget> &jump_table) {
        return prepare_code(code, jump_table, true);
    }

    vector<Line> prepare_code(vector<Line>& code, map<string, JumpTarget>& jump_table, bool do_delete_labels) {
        vector<Line> new_code;

        int line_number = 0;
        for (Line& line_struct : code) {
            string line = line_struct.value;
            stringstream line_stream(line);

            string new_line = "";
            
            bool is_jump = false;
            bool not_label = false;
            int word_number = 0;
            string word;
            while (line_stream >> word) {
                if (word.back() == ':' && !not_label) {
                    if (!do_delete_labels) {
                        new_line.append(word);
                    }
                } else {
                    not_label = true;
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
                new_code.push_back(Line{new_line, line_struct.line});
            }
            line_number++;
        }

        return new_code;
    }

    vector<Instruction> prepare_code_instructions(vector<Line>& code) {
        vector<Instruction> output;

        for (Line line : code) {
            stringstream line_stream(line.value);

            string instruction;
            line_stream >> instruction;

            int instruction_id = token_map[instruction].value;
            
            vector<Token> content;
            int j = 0;
            string word;
            while (line_stream >> word) {
                struct Token token;

                // cout << word << endl;
                // cout << (is_numeric(word) ? "true" : "false") << endl;

                // assume first word is an int because its a jump
                if (instruction_id == token_map[KEY_JUMP].value && j == 0) {
                    token.type = TYPE_INT;
                    token.int_value = stoi(word);
                    content.push_back(token);
                
                } else if (instruction_id == token_map[KEY_JUMP].value && j == 1) {
                    token.type = TYPE_INT;
                    token.int_value = comparison_map[word].value;
                    content.push_back(token);
                
                } else if (instruction_id == token_map[KEY_OP].value && j == 0) {
                    token.type = TYPE_INT;
                    token.int_value = operator_map[word].value;
                    content.push_back(token);
                
                } else if (is_numeric(word)) {
                    token.type = TYPE_DOUBLE;
                    token.double_value = stod(word);
                    content.push_back(token);
                
                } else {
                    token.type = TYPE_STRING;
                    token.string_value = word;
                    content.push_back(token);
                }
                j++;
            }
            output.push_back(Instruction{
                    line.line, 
                    instruction_id, 
                    content
                }
            );
        }

        return output;
    }
}