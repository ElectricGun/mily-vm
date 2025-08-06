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
        {KEY_END, Node{0}},
        {KEY_JUMP, Node{1}},
        {KEY_SET, Node{2}},
        {KEY_OP, Node{3}},
        {KEY_STOP, Node{4}},
        {KEY_PRINT, Node{5}},
        {KEY_PRINTFLUSH, Node{6}}
    };

    static map<string, Node> comparison_map = {
        {COMPARE_OP_EQUALS, Node{0}},
        {COMPARE_NOT_EQUAL, Node{1}},
        {COMPARE_LESS_THAN, Node{2}},
        {COMPARE_LESS_THAN_EQUALS, Node{3}},
        {COMPARE_GREATER_THAN, Node{4}},
        {COMPARE_GREATER_THAN_EQUALS, Node{5}},
        {COMPARE_STRICT_EQUALS, Node{6}},
        {KEY_ALWAYS, Node{6}}        
    };

    static map<string, Node> operator_map = {
        {OP_ADD,  Node{0}},
        {OP_MUL,  Node{1}},
        {OP_SUB,  Node{2}},
        {OP_DIV,  Node{3}},
        {OP_IDIV, Node{4}},
        {OP_MOD,  Node{5}},
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

    static function<double(double, double)> double_operation_map[] {
        {[](double a, double b) {return a + b;}},
        {[](double a, double b) {return a * b;}},
        {[](double a, double b) {return a - b;}},
        {[](double a, double b) {return a / b;}},
        {[](double a, double b) {return floor(a / b);}},
        {[](double a, double b) {return fmod(a, b);}}
    };
}