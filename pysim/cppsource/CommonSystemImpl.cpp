#include <map>

#include "Variable_p.hpp"
#include "CommonSystemImpl_p.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <boost/format.hpp>

using std::string;

namespace pysim {


CommonSystemImpl::CommonSystemImpl() :
    d_ptr(new CommonSystemImplPrivate()),
    connectionHandler(&outputs, &states, &ders)
{
}

CommonSystemImpl::~CommonSystemImpl(){
}

//////////////////////////////
//
//     Parameter handling
//
//////////////////////////////
std::vector<std::string> CommonSystemImpl::getParStringNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_strings.cbegin(); i != d_ptr->par_strings.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParMatrixNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_matrices.cbegin(); i != d_ptr->par_matrices.cend(); ++i) {
        names.push_back(i->first);
    }
    for (auto i = d_ptr->par_boost_matrices.cbegin(); i != d_ptr->par_boost_matrices.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParVectorNames() {
    std::vector<std::string> names;
    for (auto& v : d_ptr->par_vectors) {
        names.push_back(v.first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParMapNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_maps.cbegin(); i != d_ptr->par_maps.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::vector<std::string> CommonSystemImpl::getParVectorMapNames() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_vector_maps.cbegin(); i != d_ptr->par_vector_maps.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

std::string CommonSystemImpl::getParString(char* name) {
    if (d_ptr->par_strings.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    std::string s(*d_ptr->par_strings.at(name));
    return s;
}
void CommonSystemImpl::setParString(char* name, std::string value) {
    if (d_ptr->par_strings.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->par_strings.at(name)) = value;
}

std::vector<double> CommonSystemImpl::getParVector(char* name) {
    std::vector<double> out;
    if (d_ptr->par_vectors.count(name) > 0) {
        out = *d_ptr->par_vectors.at(name);
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return out;
}
void CommonSystemImpl::setParVector(char* name, std::vector<double> value) {
    if (d_ptr->par_vectors.count(name) > 0) {
        *d_ptr->par_vectors.at(name) = value;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

std::vector<std::vector<double>> CommonSystemImpl::getParMatrix(char* name) {
    std::vector<std::vector<double>> out;
    if (d_ptr->par_matrices.count(name) > 0) {
        out = *d_ptr->par_matrices.at(name);
    } else if (d_ptr->par_boost_matrices.count(name) > 0) {
        using  boost::numeric::ublas::matrix;
        matrix<double>* mat = d_ptr->par_boost_matrices[name];

        size_t columns = mat->size2();
        for (matrix<double>::const_iterator1 rowiter = mat->begin1(); rowiter != mat->end1(); rowiter++) {
            std::vector<double> row(columns);
            std::copy(rowiter.begin(), rowiter.end(), row.begin());
            out.push_back(row);
        }
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return out;
}
void CommonSystemImpl::setParMatrix(char* name, std::vector<std::vector<double>> value) {

    if (d_ptr->par_matrices.count(name) > 0) {
        *d_ptr->par_matrices.at(name) = value;
    } else if (d_ptr->par_boost_matrices.count(name) > 0) {
        boost::numeric::ublas::matrix<double> *inputm = d_ptr->par_boost_matrices.at(name);

        //Check number of rows
        if (value.size() != inputm->size1()) {
            std::string errstr = str(boost::format("Error: %1% shall contain %2% rows") % name % inputm->size1());
            throw std::invalid_argument(errstr);
        }

        //Check number of columns (for each row)
        size_t columns = inputm->size2();
        for (auto rowiter = value.cbegin(); rowiter != value.cend(); rowiter++) {
            if (rowiter->size() != columns) {
                std::string errstr = str(boost::format("Error: %1% shall contain %2% columns") % name % columns);
                throw std::invalid_argument(errstr);
            }
        }

        auto m1iter = inputm->begin1();
        for (auto rowiter = value.cbegin(); rowiter != value.cend(); rowiter++) {
            std::copy(rowiter->begin(), rowiter->end(), m1iter.begin());
            m1iter++;
        }
    } else {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
}

std::map<std::string, double> CommonSystemImpl::getParMap(char* name) {
    if (d_ptr->par_maps.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->par_maps.at(name));
}

void CommonSystemImpl::setParMap(char* name, std::map<std::string, double> value) {
    if (d_ptr->par_maps.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *d_ptr->par_maps.at(name) = value;
}

std::map<std::string, std::vector<double>> CommonSystemImpl::getParVectorMap(char* name) {
    if (d_ptr->par_vector_maps.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->par_vector_maps.at(name));
}

void CommonSystemImpl::setParVectorMap(char* name, std::map<std::string, std::vector<double>> value) {
    if (d_ptr->par_vector_maps.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *d_ptr->par_vector_maps.at(name) = value;
}

std::map<std::string, std::string> CommonSystemImpl::getParDescriptionMap() {
    return d_ptr->par_descriptions;
}


////////////////////////////////////
//
//       Connections
//
////////////////////////////////////

void CommonSystemImpl::copyoutputs() {
    connectionHandler.copyoutputs();
}

void CommonSystemImpl::copystateoutputs() {
    connectionHandler.copystateoutputs();
}


std::vector<double*> CommonSystemImpl::getStatePointers() {
    std::vector<double*> out;
    for (auto const& p : states.d_ptr->scalars) {
        out.push_back(p.second);
    }

    for (auto const&p : states.d_ptr->vectors) {
        pysim::vector* v = p.second;
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}

std::vector<double*> CommonSystemImpl::getDerPointers() {
    //In this function we iterate over the state scalars to
    //get the ders in the same order as the states.
    std::vector<double*> out;
    for (auto const& p : states.d_ptr->scalars) {
        //push the der that corresponds to the state
        out.push_back(ders.d_ptr->scalars[d_ptr->state_to_der_map_scalars[p.first]]);
    }

    for (auto const&p : states.d_ptr->vectors) {
        //the der that corresponds to the state
        pysim::vector* v = ders.d_ptr->vectors[d_ptr->state_to_der_map_vectors[p.first]];
        size_t size = v->size();
        for (size_t i = 0; i < size; ++i) {
            out.push_back(&(v->operator()(i)));
        }
    }
    return out;
}

void CommonSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);
}

//Put the state, der, input or output named "name" in the vector of pointers 
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CommonSystemImpl::store(char* name) {
    if (states.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, states.d_ptr->scalars[name]);
    } else if (ders.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, ders.d_ptr->scalars[name]);
    } else if (inputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, inputs.d_ptr->scalars[name]);
    } else if (outputs.d_ptr->scalars.count(name) == 1) {
        d_ptr->storeHandler.store_scalar(name, outputs.d_ptr->scalars[name]);
    } else if (states.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, states.d_ptr->vectors[name]);
    } else if (ders.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, ders.d_ptr->vectors[name]);
    } else if (inputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, inputs.d_ptr->vectors[name]);
    } else if (outputs.d_ptr->vectors.count(name) == 1) {
        d_ptr->storeHandler.store_vector(name, outputs.d_ptr->vectors[name]);
    } else {
        char errmsg[50];
        snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
        throw std::invalid_argument(errmsg);
    }
}

void CommonSystemImpl::add_compare_greater(char* comparename, double comparevalue) {
    using std::make_pair;

    if (outputs.d_ptr->scalars.count(comparename) == 1) {
        auto p = make_pair(outputs.d_ptr->scalars[comparename], comparevalue);
        d_ptr->compare_greater_vector.push_back(p);
    } else if (states.d_ptr->scalars.count(comparename) == 1) {
        auto p = make_pair(states.d_ptr->scalars[comparename], comparevalue);
        d_ptr->compare_greater_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}

void CommonSystemImpl::add_compare_smaller(char* comparename, double comparevalue) {
    using std::make_pair;

    if (outputs.d_ptr->scalars.count(comparename) == 1) {
        auto p = make_pair(outputs.d_ptr->scalars[comparename], comparevalue);
        d_ptr->compare_smaller_vector.push_back(p);
    } else if (states.d_ptr->scalars.count(comparename) == 1) {
        auto p = make_pair(states.d_ptr->scalars[comparename], comparevalue);
        d_ptr->compare_smaller_vector.push_back(p);
    } else {
        std::string errtxt("Could not find state or output to use for comparison");
        throw std::invalid_argument(errtxt);
    }
}


bool CommonSystemImpl::do_comparison() {
    bool is_greater = false;
    auto compare_greater_pairs = d_ptr->compare_greater_vector.begin();
    while (compare_greater_pairs != d_ptr->compare_greater_vector.end()) {
        if (*(compare_greater_pairs->first) > compare_greater_pairs->second) {
            is_greater = true;
            d_ptr->compare_greater_vector.erase(compare_greater_pairs);
        } else {
            compare_greater_pairs++;
        }
    }

    bool is_smaller = false;
    auto compare_smaller_pairs = d_ptr->compare_smaller_vector.begin();
    while (compare_smaller_pairs != d_ptr->compare_smaller_vector.end()) {
        if (*(compare_smaller_pairs->first) < compare_smaller_pairs->second) {
            is_smaller = true;
            d_ptr->compare_smaller_vector.erase(compare_smaller_pairs);
        } else {
            compare_smaller_pairs++;
        }
    }

    return is_greater || is_smaller;
}

double CommonSystemImpl::getNextUpdateTime() {
    return d_ptr->nextUpdateTime;
}

bool CommonSystemImpl::getDiscrete(){
    return d_ptr->isDiscrete;
}

StoreHandler* CommonSystemImpl::getStoreHandlerP(){
    return &(d_ptr->storeHandler);
}

}