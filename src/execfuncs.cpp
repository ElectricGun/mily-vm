#include <string>
#include "headers/execfuncs.h"
#include "headers/maps.h"

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

    void operate(string& line, map<string, GlobalVar>& active_var_map) {
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
        if (left_var.type == right_var.type) {
            out_value = double_operation_map[op](left_var.double_value, right_var.double_value);
        }

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

    void set(string& line, map<string, GlobalVar>& active_var_map) {
        stringstream line_stream(line);
        string name;
        string var_name; 
        string value;
        line_stream >> name >> var_name >> value;

        struct ActiveVar overwrite_var = get_active_var(value, active_var_map);
        string overwrite_value = overwrite_var.is_null ? "" : to_string(overwrite_var.double_value);
        
        active_var_map[var_name] = GlobalVar{overwrite_value};
    }

    void forward(int& counter, int max_lines) {
        counter++;
        if (counter >= max_lines) {
            counter = 0;
        }
    }
}