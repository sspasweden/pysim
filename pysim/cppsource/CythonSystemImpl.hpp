#pragma once
#include <vector>

#include "CommonSystemImpl.hpp"

namespace pysim{

class  CythonSystemImpl :
    public CommonSystemImpl
{
public:
    CythonSystemImpl();
    ~CythonSystemImpl();

    void preSim(){};
    void preStep();
    void doStep(double time);
    void postStep();

    double getNextUpdateTime();

    bool do_comparison();

    void* sysp;     // Pointer to python object

};

}
