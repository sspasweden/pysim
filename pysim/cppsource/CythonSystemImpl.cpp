#include "CommonSystemImpl_p.hpp"

#include "Variable_p.hpp"

#include "PysimTypes.hpp"
#include "CythonSystemImpl.hpp"
#include "../cythonsystem_api.h"

#include <iostream>
#include <string>

using std::string;

namespace pysim{

CythonSystemImpl::CythonSystemImpl()
{
    import_pysim__cythonsystem();
}

CythonSystemImpl::~CythonSystemImpl()
{
    // Delete all created inputs|outputs|states|ders
    // TODO
}

void CythonSystemImpl::preStep() {
    pre_step_callback(sysp);
}

void CythonSystemImpl::doStep(double time) {
    step_callback(sysp,time);
}

void CythonSystemImpl::postStep() {
    post_step_callback(sysp);
}

double CythonSystemImpl::getNextUpdateTime() { return 0; }

bool CythonSystemImpl::do_comparison() { return false; }


}
