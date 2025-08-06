#include <chrono>
#include <functional>
#include <string>
#include <iostream>
#include <vector>
#include "headers/constants.hpp"
#include "headers/execfuncs.hpp"
#include "headers/functions.hpp"
#include "headers/executor.hpp"

using namespace std;
using namespace mily;

namespace mily {

    static function<void(int&, int&, bool&, string&, Instruction&, map<string, ActiveVar>&)> execution_map[] {
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            c = maxl;
            forward(c, maxl);
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            jump(c, ls, avm);
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            forward(c, maxl);
            set(c, ls, avm);
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            forward(c, maxl);
            operate(c, ls, avm);
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            a = false;
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            forward(c, maxl);
            print_buffer(c, ls, pb, avm);
        }},
        {[](int& c, int& maxl, bool& a, string& pb, Instruction& ls, map<string, ActiveVar>& avm){
            forward(c, maxl);
            // TODO: target doesnt do anything
            vector<Token>& line = ls.content;
            string& target = line[0].string_value;
            cout << target << ": " << pb << endl;
            pb = "";
        }}
    }; 
    
    void execute(vector<Instruction>& code, bool verbose, bool benchmark, bool limit, int limit_amount) {
        map<string, ActiveVar> active_var_map;
        double memory[512];

        int MAX_LINE = code.size();
        int counter = 0;
        int iteration = 0;
        bool active = true;
        string printbuffer;

        string thingy =  "-------------------------------------------";

        // time start
        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        
        while ((limit ? iteration < limit_amount : true) && active) {
            Instruction& instruction = code[counter];
            int& instruction_id = instruction.id;
            vector<Token>& content = instruction.content;
            
            if (verbose) {
                string out = to_string(instruction_id) + " ";
                for (Token s : instruction.content) {
                    if (s.type == TYPE_STRING)
                        out.append(s.string_value).append(" ");
                    
                    else if (s.type == TYPE_DOUBLE)
                        out.append(to_string(s.double_value)).append(" ");

                    else if (s.type == TYPE_INT)
                        out.append(to_string(s.int_value)).append(" ");

                    else if (s.type == TYPE_NULL)
                        out.append("null").append(" ");
                }
                // cout << instruction.line << ": " << instruction.line << " " << out << endl;
                cout << endl;
                cout << "ITERATION: " << iteration << "  LINE: " << instruction.line << "  COUNTER: " << counter <<  "\n" << out << endl;
                cout << endl << "VARIABLES:" << endl;
                for (auto& entry : active_var_map) {
                    cout << entry.first << ": " << entry.second.double_value << endl;
                }
                cout << thingy << endl;
            }
            
            execution_map[instruction_id](counter, MAX_LINE, active, printbuffer, instruction, active_var_map);
            iteration ++;
        }
        // time end
        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        double nanoseconds = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        
        if (benchmark) {
            cout << fixed << nanoseconds / 1000000 << " seconds to run " << iteration << " instructions" << endl;
            cout << fixed << (double) iteration / (nanoseconds / 1000000.0) << " instructions per second" << endl;
        }
    }
}