#pragma once

#include <cmath>
#include <functional>
#include <string>
#include "constants.hpp"
#include "functions.hpp"

using namespace std;
using namespace mily;

namespace mily {

    static map<string, Node> token_map = {
        {"end", Node{0}},
        {"set", Node{1}},
        {"op", Node{2}},
        {"jump", Node{3}}
    };

    static map<string, function<bool(double, double)>> double_compare_map {
        {COMPARE_OP_EQUALS, 
            [](double a, double b) {return a == b;}},
        {COMPARE_NOT_EQUAL, 
            [](double a, double b) {return a != b;}},
        {COMPARE_LESS_THAN, 
            [](double a, double b) {return a < b;}},
        {COMPARE_LESS_THAN_EQUALS, 
            [](double a, double b) {return a <= b;}},
        {COMPARE_GREATER_THAN, 
            [](double a, double b) {return a > b;}},
        {COMPARE_GREATER_THAN_EQUALS, 
            [](double a, double b) {return a >= b;}},
            // TODO: strict equals isnt different from equals
        {COMPARE_STRICT_EQUALS, 
            [](double a, double b) {return a == b;}}
    };

    static map<string, function<double(double, double)>> double_operation_map {
        {OP_ADD, 
            [](double a, double b) {return a + b;}},
        {OP_MUL, 
            [](double a, double b) {return a * b;}},
        {OP_SUB, 
            [](double a, double b) {return a - b;}},
        {OP_DIV, 
            [](double a, double b) {return a / b;}},
        {OP_IDIV, 
            [](double a, double b) {return floor(a / b);}},
        {OP_MOD, 
            [](double a, double b) {return fmod(a, b);}}
    };
}