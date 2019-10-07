//   Copyright (c) 2014-2016 SSPA Sweden AB

#pragma once

#include "CommonSystemImpl.hpp"
#include "PysimTypes.hpp"
#include <Eigen/Dense>

#include <vector>
#include <map>
#include <string>

namespace pysim {

class CppSystem :
    public CommonSystemImpl
{
public:
    CppSystem();
    virtual ~CppSystem();

    //  Inherited from System
    virtual void preSim(){};
    virtual void preStep() {};
    virtual void doStep(double time)=0;
    virtual void postStep() {};

protected:

    ///////////////////////////////////////
    //
    //      Cpp Interface
    //
    ///////////////////////////////////////

    void par(std::vector<double>* vars, const char* name, const char* desc);
    void par(std::vector<std::vector<double>>* vars, const char* name, const char* desc);
    void par(std::string*, const char* name, const char* description);
    void par(std::map<std::string, double>*, const char* name, const char* description);
    void par(std::map<std::string, std::vector<double>>* var, const char* name, const char* description);

    void setNextUpdate(double t);
    void setDiscrete(bool d = true);

};

#define STATE(x,dx,s) { states.add(#x,&x,s); ders.add(#dx,&dx,s); };

#define INPUT(x,s) inputs.add(#x,&x,s);
#define OUTPUT(x,s) outputs.add(#x,&x,s);

#define PAR(x,s) par(&x,#x,s);
}