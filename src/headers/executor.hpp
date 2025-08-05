#pragma once

#include "functions.hpp"
#include <vector>

using namespace std;

namespace mily {

    void execute(vector<Line>& code, bool verbose, bool benchmark, bool limit);
}