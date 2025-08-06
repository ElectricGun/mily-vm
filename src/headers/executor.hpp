#pragma once

#include "functions.hpp"
#include <vector>

using namespace std;

namespace mily {

    void execute(vector<Instruction>& code, bool verbose, bool benchmark, bool limit);
}