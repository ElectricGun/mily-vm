#pragma once

#include <map>
#include <string>
#include "functions.hpp"

using namespace std;

namespace mily {

    bool compare(ActiveVar& left, string& comparator, ActiveVar& right);

    ActiveVar parse_to_active_var(int& counter, Token& token, map<string, ActiveVar>& active_var_map);

    void operate(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map);

    void jump(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map);

    void set(int& counter, Instruction& instruction, map<string, ActiveVar>& active_var_map);

    void print_buffer(int& counter, Instruction& instruction, string& printbuffer, map<string, ActiveVar>& active_var_map);

    void forward(int& counter, int& max_lines);
}