#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include "headers/execfuncs.hpp"
#include "headers/constants.hpp"
#include "headers/maps.hpp"

using namespace std;
using namespace mily;

namespace mily {

    bool compare(ActiveVar left, string comparator, ActiveVar right) {

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

    ActiveVar get_active_var(int& counter, string& token, map<string, GlobalVar>& active_var_map) {
        struct ActiveVar out;

        if (token == VAR_COUNTER) {
            out.double_value = counter;
            out.is_null = false;
        
        } else if (is_numeric(token)) {
            out.double_value = stod(token);
            out.is_null = false;
        
        } else if (!active_var_map[token].value.empty()) {
            out.double_value = stod(active_var_map[token].value);
            out.is_null = false;
        }
        return out;
    }

    void operate(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map) {
        string op = line[0];
        string overwrite = line[1];
        string left = line[2];
        string right = line[3];

        struct GlobalVar out;
        struct ActiveVar left_var = get_active_var(counter, left, active_var_map);
        struct ActiveVar right_var = get_active_var(counter, right, active_var_map);

        // cout << "OPERATION: " << overwrite << endl;
        // cout << (left_var.is_null ? "null" : "double: " + to_string(left_var.double_value)) << endl;
        // cout << (right_var.is_null ? "null" : "double: " + to_string(right_var.double_value)) << endl;

        double out_value;
        if (left_var.type == right_var.type) {
            out_value = double_operation_map[op](left_var.double_value, right_var.double_value);
        
        } else {
            cerr << "Type mismatch" << endl;
            exit(EXIT_FAILURE);
        }

        if (overwrite == VAR_COUNTER) {
            counter = (int) out_value;
            return;
        }

        // cout << "RESULT: " << out_value << endl;

        out.value = to_string(out_value);
        active_var_map[overwrite] = out;
    }

    void jump(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map) {
        int target = stoi(line[0]);
        string op = line[1];

        if (op == KEY_ALWAYS) {
            counter = target;
            return;
        }

        string left = line[2];
        string right = line[3];

        struct ActiveVar left_var = get_active_var(counter, left, active_var_map);
        struct ActiveVar right_var = get_active_var(counter, right, active_var_map);

        // cout << "JUMP" << endl;
        // cout << (left_var.is_null ? "null" : "double: " + to_string(left_var.double_value)) << endl;
        // cout << (right_var.is_null ? "null" : "double: " + to_string(right_var.double_value)) << endl;

        if (compare(left_var, op, right_var)) {
            counter = target;
            // cout << "RESULT: " << "true" << endl;

        } else {
            // cout << "RESULT: " << "false" << endl;
            counter++;
        }
    }

    void set(int& counter, vector<string>& line, map<string, GlobalVar>& active_var_map) {
        string var_name = line[0]; 
        string value = line[1];
        struct ActiveVar overwrite_var = get_active_var(counter, value, active_var_map);

        if (var_name == VAR_COUNTER) {
            counter = (int) overwrite_var.double_value;
            return;
        }
        string overwrite_value = overwrite_var.is_null ? "" : to_string(overwrite_var.double_value);
        active_var_map[var_name] = GlobalVar{overwrite_value};
    }

    void print_buffer(int& counter, vector<string>& line, string& printbuffer, map<string, GlobalVar>& active_var_map) {
        bool iterated = false;
        bool string_open = false;
        bool string_started = false;
        bool string_closed = false;

        string input_str;
        int size = line.size();
        for (int i = 0; i < size; i++) {
            input_str.append(line[i]);
            if (i < size - 1) {
                input_str.push_back(' ');
            }
        }
        
        string buffer;
        string target_var;
        string prev_str;
        // this can be improved to be more efficient, but it doesnt seem to hinder speed too much right now
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
                if (str == "n" && prev_str == "\\") {
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
            struct ActiveVar active_var = get_active_var(counter, buffer, active_var_map);
            if (active_var.is_null) {
                printbuffer.append("null");
                return;
            } else {
                printbuffer.append(to_string(active_var.double_value));
                return;
            }
        }
    }

    void forward(int& counter, int max_lines) {
        counter++;
        if (counter >= max_lines) {
            counter = 0;
        }
    }
}