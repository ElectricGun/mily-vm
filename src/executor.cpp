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

    static map<string, function<void(int&, int, bool&, string&, string&, map<string, GlobalVar>&)>> execution_map {
        {KEY_END, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            c = maxl;
            forward(c, maxl);
        }},
        {KEY_JUMP, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            jump(c, ls, avm);
        }},
        {KEY_SET, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            set(c, ls, avm);
        }},
        {KEY_OP, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            operate(c, ls, avm);
        }},
        {KEY_STOP, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            a = false;
        }},
        {KEY_PRINT, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            print_buffer(c, ls, pb, avm);
        }},
        {KEY_PRINTFLUSH, [](int& c, int maxl, bool& a, string& pb, string& ls, map<string, GlobalVar>& avm){
            forward(c, maxl);
            string name;
            // TODO: target doesnt do anything
            string target;
            stringstream line_stream(ls);
            line_stream >> name >> target;
            cout << pb << endl;
            pb = "";
        }}
    }; 
    
    void execute(vector<Line>& code, bool verbose, bool benchmark, bool limit) {
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
            Line line = code[counter];
            string line_string = line.value;
            stringstream line_stream(line_string);
            
            if (verbose) {
                cout << endl;
                cout << "ITERATION: " << iteration << "  LINE: " << line.line << "  COUNTER: " << counter <<  "\n" << line_string << endl;
                cout << endl << "VARIABLES:" << endl;
                for (auto& entry : active_var_map) {
                    cout << entry.first << ": " << entry.second.value << endl;
                }
                cout << thingy << endl;
            }
            string word;
            line_stream >> word;
            execution_map[word](counter, MAX_LINE, active, printbuffer, line_string, active_var_map);
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