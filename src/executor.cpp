#include <chrono>
#include <functional>
#include <sstream>
#include <string>
#include <iostream>
#include "headers/execfuncs.hpp"
#include "headers/executor.hpp"

using namespace std;
using namespace mily;

namespace mily {

    static map<string, function<void(int&, int, bool&, string&, map<string, GlobalVar>&)>> execution_map {
        {KEY_END, [](int& c, int maxl, bool& a, string& ls, map<string, GlobalVar>& avm){
            c = 0;
        }},
        {KEY_JUMP, [](int& c, int maxl, bool& a, string& ls, map<string, GlobalVar>& avm){
            jump(c, ls, avm);
        }},
        {KEY_SET, [](int& c, int maxl, bool& a, string& ls, map<string, GlobalVar>& avm){
            set(ls, avm);
            forward(c, maxl);
        }},
        {KEY_OP, [](int& c, int maxl, bool& a, string& ls, map<string, GlobalVar>& avm){
            operate(ls, avm);
            forward(c, maxl);
        }},
        {KEY_STOP, [](int& c, int maxl, bool& a, string& ls, map<string, GlobalVar>& avm){
            a = false;
        }}
    }; 
    
    void execute(vector<string>& code, bool verbose, bool benchmark) {
        map<string, GlobalVar> active_var_map;
        double memory[512];

        int const MAX_LINE = code.size();
        int counter = 0;
        int iteration = 0;
        bool active = true;

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();        
        while ((benchmark ? iteration < 1000000 : true) && active) {
            string line_string = code[counter];
            stringstream line_stream(line_string);
            
            if (verbose) {
                cout << "ITERATION: " << iteration << endl;
                cout << "LINE: " << counter <<  "\n" << line_string << endl;    
                for (auto& entry : active_var_map) {
                    cout << entry.first << ": " << entry.second.value << endl;
                }
                cout << endl;
            }
            string word;
            line_stream >> word;
            execution_map[word](counter, MAX_LINE, active, line_string, active_var_map);
            iteration ++;
        }

        chrono::steady_clock::time_point end = chrono::steady_clock::now();
        double nanoseconds = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        
        cout << fixed << nanoseconds / 1000000 << " seconds to run " << iteration << " instructions" << endl;
        cout << fixed << (double) iteration / (nanoseconds / 1000000.0) << " instructions per second" << endl;
    }
}