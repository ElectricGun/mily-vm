#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.hpp"

using namespace std;

namespace mily {

    void execute(vector<string>& code, bool verbose, bool benchmark);
}