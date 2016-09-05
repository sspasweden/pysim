//   Copyright (c) 2014-2016 SSPA Sweden AB
#pragma once

#include <vector>
#include <map>
#include <string>

#include "PysimTypes.hpp"
#include "Variable.hpp"

namespace pysim {

struct ConnectionHandlerPrivate;
class CommonSystemImpl;
class CompositeSystemImpl;

class ConnectionHandler{
public:
    ConnectionHandler(Variable* outputp, Variable* statep = nullptr);
    virtual ~ConnectionHandler();

    void copyoutputs();
    void copystateoutputs();

    template <typename T>
    void connect(char* outputname, T* inputsys, char* inputname);

    //void connect(char* outputname, CommonSystemImpl* inputsys, char* inputname);
    //void connect(char* outputname, CompositeSystemImpl* inputsys, char* inputname);

protected:

private:
    std::unique_ptr<ConnectionHandlerPrivate> d_ptr;

};

}