#include "ScalarAdder.h"

#include "factory.hpp"
#include <boost/format.hpp>

using std::string;

string ScalarAdder::getDocs() {
    std::string desc = str(boost::format("Adds inputs together. \n The inputs are named input_0, input_1, ..."));
    return desc;
}

ScalarAdder::ScalarAdder(void) :
    _inputs(MAX_INPUTS,0.0),
    out(0.0)
{
    int i = 0;
    for (auto iter = _inputs.begin(); iter != _inputs.end(); ++iter) {
        double* p = &(*iter);
        std::string name = str(boost::format("input_%1%") % i++);
        inputs.add(name.c_str(), p, "");
    }
    outputs.add("out", &out, "The sum of all inputs");
}

void ScalarAdder::doStep(double time) {
    out = 0;
    for (auto iter = _inputs.begin(); iter != _inputs.end(); ++iter) {
        out += *iter;
    }
}

REGISTER_SYSTEM(ScalarAdder);
