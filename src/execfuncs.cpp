#include <cstdlib>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "headers/execfuncs.hpp"
#include "headers/constants.hpp"
#include "headers/maps.hpp"
#include "headers/functions.hpp"

using namespace std;
using namespace mily;

namespace mily {

    bool compare(ActiveVar& left, int& comparator, ActiveVar& right) {

        // TODO: this makes it strictequal which might not be good
        if (left.type != right.type) {
            return false;
        
        } else if (left.type == TYPE_DOUBLE) {
            return double_compare_map[comparator](left.double_value, right.double_value);

        } else {
            cerr << "Unknown type in compare(): \"" << left.type << "\"" << endl;
        }

        return false;
    }

    ActiveVar parse_to_active_var(int& counter, Token& token, map<string, ActiveVar>& active_var_map) {
        struct ActiveVar out;

        // cout << token.type << " " << token.string_value << endl;

        if (token.type == TYPE_DOUBLE) {
            out.double_value = token.double_value;
            out.type = TYPE_DOUBLE;
            return out;

        } else if (token.string_value == VAR_COUNTER) {
            out.double_value = counter;
            out.type = TYPE_DOUBLE;
            
        } else if (active_var_map[token.string_value].type == TYPE_DOUBLE) {
            return active_var_map[token.string_value];
        
        } else {
            cerr << "Key not found in variable map: " << token.string_value << endl;
        } 
        return out;
    }

    void operate(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map) {
        vector<Token>& line = instruction.content;
        int& op = line[0].int_value;
        string& overwrite = line[1].string_value;

        Token& left = line[2];
        Token& right = line[3];

        struct ActiveVar out;
        struct ActiveVar left_var = parse_to_active_var(counter, left, active_var_map);
        struct ActiveVar right_var = parse_to_active_var(counter, right, active_var_map);

        double out_value;
        if (left_var.type == right_var.type) {
            out_value = double_operation_map[op](left_var.double_value, right_var.double_value);
        
        } else {
            cerr << "Type mismatch" << " " << left_var.type << " " << right_var.type << " " << endl;
            cerr << left_var.type << " " << right_var.type << endl;
            cerr << left_var.double_value << " " << right_var.double_value << endl;

            exit(EXIT_FAILURE);
        }

        if (overwrite == VAR_COUNTER) {
            counter = (int) out_value;
            return;
        }

        out.double_value = out_value;
        out.type = TYPE_DOUBLE;
        active_var_map[overwrite] = out;
    }

    void jump(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map) {
        vector<Token>& line = instruction.content;

        int target = line[0].int_value;
        int op = line[1].int_value;

        if (op == MAP_VALUE_ALWAYS) {
            counter = target;
            return;
        }

        Token& left = line[2];
        Token& right = line[3];
        struct ActiveVar left_var = parse_to_active_var(counter, left, active_var_map);
        struct ActiveVar right_var = parse_to_active_var(counter, right, active_var_map);

        if (compare(left_var, op, right_var)) {
            counter = target;

        } else {
            counter++;
        }
    }

    void set(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map) {
        vector<Token>& line = instruction.content;
        string& var_name = line[0].string_value; 
        
        Token& value = line[1];
        struct ActiveVar overwrite_var = parse_to_active_var(counter, value, active_var_map);

        if (var_name == VAR_COUNTER) {
            counter = (int) overwrite_var.double_value;
            return;
        }
        // cout << var_name << endl;

        active_var_map[var_name] = overwrite_var;
    }

    void print_buffer(int& counter, Instruction& instruction, string& printbuffer, map<string, ActiveVar>& active_var_map) {
        vector<Token>& line = instruction.content;
        Token tok = line[0];

        bool iterated = false;
        bool string_open = false;
        bool string_started = false;
        bool string_closed = false;

        if (tok.type == TYPE_DOUBLE) {
            printbuffer.append(to_string(tok.double_value));
            return;

        } else if (tok.type == TYPE_INT) {
            printbuffer.append(to_string(tok.int_value  ));
            return;
        }

        string input_str;
        int size = line.size();
        for (int i = 0; i < size; i++) {
            input_str.append(line[i].string_value);
            if (i < size - 1) {
                input_str.push_back(' ');
            }
        }

        string buffer;
        string target_var;
        string prev_str;
        bool punc_escaped = false;
        // TODO: this can be improved to be more efficient
        for (int i = 0; i < input_str.length(); i++) {
            string str(1, input_str[i]);

            if (string_started && string_closed) {
                printbuffer.append("null");
                return;

            } else if (str == PUNC_SPEECH) {
                if (!string_started) {
                    string_started = true;
                    string_open = true;
                
                } else if ((iterated && !string_started)) {
                    printbuffer.append("null");
                    return;
                
                } else if (string_open) {
                    string_closed = true;
                }
            } else {
                if (str == "n" && prev_str == PUNC_ESCAPE) {
                    buffer.pop_back();
                    buffer.push_back('\n');
                } else {
                    buffer.push_back(input_str[i]);
                }
            }
            prev_str = str;
            iterated = true;
        }


        if (string_started) {
            printbuffer.append(buffer);
        
        } else {
            struct Token token = Token{TYPE_STRING, 0, buffer};
            struct ActiveVar active_var = parse_to_active_var(counter, token, active_var_map);
            if (active_var.type == TYPE_NULL) {
                printbuffer.append("null");
                return;
            } else {
                printbuffer.append(to_string(active_var.double_value));
                return;
            }
        }
    }

    void forward(int& counter, int& max_lines) {
        counter++;
        counter %= max_lines;
    }
}