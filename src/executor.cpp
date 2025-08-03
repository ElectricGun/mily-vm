#include <chrono>
#include <sstream>
#include <string>
#include <iostream>
#include "headers/maps.h"
#include "headers/constants.h"
#include "headers/executor.h"

using namespace std;
using namespace maps;

namespace executor {
    bool is_numeric(string line) {
        char* p;
        strtol(line.c_str(), &p, 10);
        return *p == 0;
    }

    bool compare(ActiveVar left, string comparator, ActiveVar right) {

        // TODO: this makes it strictequal which might not be good
        if (left.type != right.type) {
            return false;
        
        } else if (left.type == TYPE_DOUBLE) {
            // if (comparator == COMPARE_OP_EQUALS) {
            //     return left.double_value == right.double_value;

            // } else if (comparator == COMPARE_NOT_EQUAL) {
            //     return left.double_value != right.double_value;

            // } else if (comparator == COMPARE_LESS_THAN) {
            //     return left.double_value < right.double_value;

            // } else if (comparator == COMPARE_LESS_THAN_EQUALS) {
            //     return left.double_value <= right.double_value;

            // } else if (comparator == COMPARE_GREATER_THAN) {
            //     return left.double_value > right.double_value;

            // } else if (comparator == COMPARE_GREATER_THAN_EQUALS) {
            //     return left.double_value >= right.double_value;

            // } else if (comparator == COMPARE_STRICT_EQUALS) {
            //     return left.double_value == right.double_value;

            // } else {
            //     cerr << "Unknown operator in compare(): \"" << comparator << "\"" << endl;
            // }
            return double_compare_map[comparator](left.double_value, right.double_value);

        } else {
            cerr << "Unknown type in compare(): \"" << left.type << "\"" << endl;
        }

        return false;
    }

    ActiveVar get_active_var(string& token, map<string, GlobalVar>& active_var_map) {
        struct ActiveVar out;

        if (is_numeric(token)) {
            out.double_value = stod(token);
            out.is_null = false;
        
        } else if (!active_var_map[token].value.empty()) {
            out.double_value = stod(active_var_map[token].value);
            out.is_null = false;
        }

        return out;
    }

    void operate(map<string, GlobalVar>& active_var_map, string& line) {
        string name;
        string op;
        string overwrite;
        string left;
        string right;
        stringstream line_stream(line);
        line_stream >> name >> op >> overwrite >> left >> right;

        struct GlobalVar out;
        struct ActiveVar left_var = get_active_var(left, active_var_map);
        struct ActiveVar right_var = get_active_var(right, active_var_map);

        // cout << "OPERATION: " << overwrite << endl;
        // cout << (left_var.is_null ? "null" : "double: " + to_string(left_var.double_value)) << endl;
        // cout << (right_var.is_null ? "null" : "double: " + to_string(right_var.double_value)) << endl;

        double out_value;
        if (left_var.type != right_var.type) {
            out_value = double_operation_map[op](left_var.double_value, right_var.double_value);
        }
        // if (left_var.type != right_var.type) {
        //     cerr << "Invalid types \"" << left_var.type << "\" and \"" << right_var.type << "\" in operate()" << endl;

        // } else if (op == OP_ADD) {
        //     out_value = left_var.double_value + right_var.double_value;

        // } else if (op == OP_MUL) {
        //     out_value = left_var.double_value * right_var.double_value;

        // } else if (op == OP_SUB) {
        //     out_value = left_var.double_value - right_var.double_value;

        // } else if (op == OP_DIV) {
        //     out_value = left_var.double_value / right_var.double_value;

        // } else if (op == OP_IDIV) {
        //     out_value = floor(left_var.double_value / right_var.double_value);

        // } else if (op == OP_MOD) {
        //     out_value = fmod(left_var.double_value, right_var.double_value);

        // } else {
        //     cerr << "Invalid operator \"" << op << "\" in operate()" << endl;

        // }

        // cout << "RESULT: " << out_value << endl;

        out.value = to_string(out_value);

        active_var_map[overwrite] = out;
    }

    void jump(int& counter, string& line, map<string, GlobalVar>& active_var_map) {
        string name;
        int target;
        string op;
        string left;
        string right;
        istringstream stream(line);
        stream >> name >> target >> op >> left >> right;

        if (op == KEY_ALWAYS) {
            counter = target;
            return;
        }

        struct ActiveVar left_var = get_active_var(left, active_var_map);
        struct ActiveVar right_var = get_active_var(right, active_var_map);

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

    void set(map<string, GlobalVar>& active_var_map, string& line) {
        stringstream line_stream(line);
        string name;
        string var_name; 
        string value;
        line_stream >> name >> var_name >> value;

        struct ActiveVar overwrite_var = get_active_var(value, active_var_map);
        string overwrite_value = overwrite_var.is_null ? "" : to_string(overwrite_var.double_value);
        
        active_var_map[var_name] = GlobalVar{overwrite_value};
    }

    void execute(vector<string> code, bool verbose) {
        map<string, GlobalVar> active_var_map;
        double memory[1024];

        int const MAX_LINE = code.size();
        int counter = 0;
        int iteration = 0;

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        
        while (iteration < 1000000) {
            string line_string = code[counter];
            stringstream line_stream(line_string);
            bool do_continue = false;

            if (verbose) {
                cout << "ITERATION: " << iteration << endl;
                cout << "LINE: " << counter <<  "\n" << line_string << endl;    
                for (auto& entry : active_var_map) {
                    cout << entry.first << ": " << entry.second.value << endl;
                }
                cout << endl;
            }

            string word;
            string type;
            while (line_stream >> word) {
                if (word == KEY_END) {
                    counter = 0;
                    do_continue = true;
                
                } else if (word == KEY_JUMP) {
                    type = KEY_JUMP;
                    jump(counter, line_string, active_var_map);
                    do_continue = true;
                
                } else if (word == KEY_SET) {
                    set(active_var_map, line_string);
                
                } else if (word == KEY_OP) {
                    operate(active_var_map, line_string);
                }
            }
            iteration ++;
            
            if (do_continue) {
                continue;
            }
            counter++;

            if (counter >= MAX_LINE) {
                counter = 0;
            }
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        double nanoseconds = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        
        cout << nanoseconds << " nanoseconds to run 1000_000 instructions" << endl;
        cout << 1000000.0 / (nanoseconds / 1000000.0) << " instructions per second" << endl;
    }
}