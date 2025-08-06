#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include "headers/constants.hpp"
#include "headers/execfuncs.hpp"
#include "headers/functions.hpp"
#include "headers/executor.hpp"

using namespace std;
using namespace mily;

namespace mily {

    static function<void(int&, int, bool&, string&, vector<string>&, map<string, GlobalVar>&)> execution_map[] {
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            c = maxl;
            forward(c, maxl);
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            jump(c, ls, avm);
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            set(c, ls, avm);
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            operate(c, ls, avm);
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            a = false;
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            print_buffer(c, ls, pb, avm);
        }},
        {[](int& c, int maxl, bool& a, string& pb, vector<string>& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            // TODO: target doesnt do anything
            string target = ls[0];
            cout << pb << endl;
            pb = "";
        }}
    }; 
    
    void execute(vector<Instruction>& code, bool verbose, bool benchmark, bool limit) {
        map<string, GlobalVar> active_var_map;
        double memory[512];

        int const MAX_LINE = code.size();
        int counter = 0;
        int iteration = 0;
        bool active = true;
        string printbuffer;

        string thingy =  "-------------------------------------------";

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();        
        while ((limit ? iteration < 1000000 : true) && active) {
            Instruction instruction = code[counter];
            int instruction_id = instruction.id;
            vector<string> content = instruction.content;
            
            if (verbose) {
                string out = to_string(instruction_id) + " ";
                for (string s : instruction.content) {
                    out.append(s).append(" ");
                }
                cout << instruction.line << ": " << instruction.line << " " << out << endl;
                cout << endl;
                cout << "ITERATION: " << iteration << "  LINE: " << instruction.line << "  COUNTER: " << counter <<  "\n" << out << endl;
                cout << endl << "VARIABLES:" << endl;
                for (auto& entry : active_var_map) {
                    cout << entry.first << ": " << entry.second.value << endl;
                }
                cout << thingy << endl;
            }
            
            execution_map[instruction_id](counter, MAX_LINE, active, printbuffer, content, active_var_map);
            iteration ++;
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        double nanoseconds = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        
        if (benchmark) {
            cout << fixed << nanoseconds / 1000000 << " seconds to run " << iteration << " instructions" << endl;
            cout << fixed << (double) iteration / (nanoseconds / 1000000.0) << " instructions per second" << endl;
        }
    }
}