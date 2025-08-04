#pragma once

#include "functions.h"
#include <string>

using namespace std;

namespace mily {
    
    static const string 
        KEY_JUMP = "jump",
        KEY_OP = "op",
        KEY_SET = "set",
        KEY_END = "end",

        // operators
        OP_ADD = "add",
        OP_MUL = "mul",
        OP_SUB = "sub",
        OP_DIV = "div",
        OP_POW = "pow",
        OP_IDIV = "idiv",
        OP_MOD = "mod",
        COMPARE_OP_EQUALS = "equal",
        COMPARE_NOT_EQUAL = "notEqual",
        COMPARE_LESS_THAN = "lessThan",
        COMPARE_LESS_THAN_EQUALS = "lessThanEq",
        COMPARE_GREATER_THAN = "greaterThan",
        COMPARE_GREATER_THAN_EQUALS = "greaterThanEq",
        COMPARE_STRICT_EQUALS = "strictEqual",

        KEY_ALWAYS = "always",
        KEY_TRUE = "true",
        KEY_FALSE = "false",

        TYPE_DOUBLE = "double",
        TYPE_STRING = "string"
        ;
}