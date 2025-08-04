#pragma once

#include <map>
#include <string>
#include <vector>
#include "constants.h"

using namespace std;

namespace mily {

    void execute(vector<string>& code, bool verbose, bool benchmark);
}