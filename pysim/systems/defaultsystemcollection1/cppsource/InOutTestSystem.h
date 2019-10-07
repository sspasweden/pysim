#pragma once
#include "CppSystem.hpp"

class InOutTestSystem :
    public pysim::CppSystem
{
public:
    InOutTestSystem(void);

    static std::string getDocs();

    //First calculation in a simulation, only done once
    void preSim(){};

    //This function is called for each evaluation
    void doStep(double time);

    //After each set of evaluations when a new state value 
    //has been calculated this section is called
    void postStep(){};

    //Before finishing the simulation this function is called
    void finally(){};

protected:

    double input_scalar;
    pysim::vector input_vector;
    pysim::matrix input_matrix;

    double input_output_scalar;
    pysim::vector input_output_vector;
    pysim::matrix input_output_matrix;

    double state_output_scalar;
    pysim::vector state_output_vector;
    pysim::matrix state_output_matrix;

    double state_scalar;
    pysim::vector state_vector;
    pysim::matrix state_matrix;

    double der_scalar;
    pysim::vector der_vector;
    pysim::matrix der_matrix;

    Eigen::Matrix3d testMatrix;
};
