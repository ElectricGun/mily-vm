#include <chrono>
#include <sstream>
#include <string>
#include <iostream>
#include "headers/execfuncs.h"
#include "headers/executor.h"
#include "headers/maps.h"

using namespace std;
using namespace mily;

namespace mily {
    
    void execute(vector<string>& code, bool verbose, bool benchmark) {
        map<string, GlobalVar> active_var_map;
        double memory[1024];

        int const MAX_LINE = code.size();
        int counter = 0;
        int iteration = 0;

        chrono::steady_clock::time_point begin = chrono::steady_clock::now();
        
        while (benchmark ? iteration < 1000000 : true) {
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
            line_stream >> word;
            instruction_map[word](counter, do_continue, line_string, active_var_map);
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
        
        cout << fixed << nanoseconds / 1000000.0 << " seconds to run 1 million instructions" << endl;
        cout << fixed << 1000000.0 / (nanoseconds / 1000000.0) << " instructions per second" << endl;
    }
}