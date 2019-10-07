#include "Adder6D.h"

#include "factory.hpp"
#include <boost/numeric/ublas/assignment.hpp>
#include <boost/format.hpp>

REGISTER_SYSTEM(Adder6D);
using namespace std;

string Adder6D::getDocs() {
	return string(
		"System that simply adds two vectors together\n\n"
		);
}

Adder6D::Adder6D(void) :
	_inputs(MAX_INPUTS, pysim::vector::Zero(6)),
	out(pysim::vector::Zero(6))
{
	int i = 0;
	for (auto iter = _inputs.begin(); iter != _inputs.end(); ++iter) {
		pysim::vector* p = &(*iter);
		std::string name = str(boost::format("input_%1%") % i++);
		inputs.add(name.c_str(), p, "");
	}

	OUTPUT(out, "Result of the addition")
}

void Adder6D::doStep(double time) {
	out << 0, 0, 0, 0, 0, 0;
	for (auto iter = _inputs.begin(); iter != _inputs.end(); ++iter) {
		out += *iter;
	}
}
