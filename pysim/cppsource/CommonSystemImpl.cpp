#include <map>

#include "Variable_p.hpp"
#include "CommonSystemImpl_p.hpp"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string>
#include <boost/format.hpp>
#include <boost/range/algorithm_ext.hpp>

using std::string;

namespace pysim {


CommonSystemImpl::CommonSystemImpl() :
    connectionHandler(&inputs, &outputs, &states, &ders),
    d_ptr(new CommonSystemImplPrivate())
{
}

CommonSystemImpl::~CommonSystemImpl(){
}

//////////////////////////////
//
//     Parameter handling
//
//////////////////////////////

template<>
std::vector<std::string> CommonSystemImpl::getParNames<ParString>() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_strings.cbegin(); i != d_ptr->par_strings.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

template <>
std::vector<std::string> CommonSystemImpl::getParNames<ParMatrix>() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_matrices.cbegin(); i != d_ptr->par_matrices.cend(); ++i) {
        names.push_back(i->first);
    }

    return names;
}

template <>
std::vector<std::string> CommonSystemImpl::getParNames<ParVector>() {
    std::vector<std::string> names;
    for (auto& v : d_ptr->par_vectors) {
        names.push_back(v.first);
    }
    return names;
}


template <>
std::vector<std::string> CommonSystemImpl::getParNames<ParMap>(){
    std::vector<std::string> names;
    for (auto i = d_ptr->par_maps.cbegin(); i != d_ptr->par_maps.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

template <>
std::vector<std::string> CommonSystemImpl::getParNames<ParVectorMap>() {
    std::vector<std::string> names;
    for (auto i = d_ptr->par_vector_maps.cbegin(); i != d_ptr->par_vector_maps.cend(); ++i) {
        names.push_back(i->first);
    }
    return names;
}

template <>
std::string CommonSystemImpl::getPar(char* name) {
    if (d_ptr->par_strings.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    std::string s(*d_ptr->par_strings.at(name));
    return s;
}

template <>
void CommonSystemImpl::setPar(char* name, std::string value) {
    if (d_ptr->par_strings.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *(d_ptr->par_strings.at(name)) = value;
}

template <>
ParVector CommonSystemImpl::getPar(char* name) {
    std::vector<double> out;
    if (d_ptr->par_vectors.count(name) > 0) {
        out = *d_ptr->par_vectors.at(name);
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return out;
}

template <>
void CommonSystemImpl::setPar(char* name, ParVector value) {
    if (d_ptr->par_vectors.count(name) > 0) {
        *d_ptr->par_vectors.at(name) = value;
    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
}

template <>
ParMatrix CommonSystemImpl::getPar(char* name) {
    std::vector<std::vector<double>> out;
    if (d_ptr->par_matrices.count(name) > 0) {
        out = *d_ptr->par_matrices.at(name);

    } else {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return out;
}

template <>
void CommonSystemImpl::setPar(char* name, ParMatrix value) {
    if (d_ptr->par_matrices.count(name) > 0) {
        *d_ptr->par_matrices.at(name) = value;

    } else {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
}

template <>
ParMap CommonSystemImpl::getPar(char* name) {
    if (d_ptr->par_maps.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->par_maps.at(name));
}

template <>
void CommonSystemImpl::setPar(char* name, ParMap value) {
    if (d_ptr->par_maps.count(name) < 1) {
        char errmsg[50];
        snprintf(errmsg, 50, "Could not find: %s", name);
        throw std::invalid_argument(errmsg);
    }
    *d_ptr->par_maps.at(name) = value;
}

template <>
ParVectorMap CommonSystemImpl::getPar(char* name) {
    if (d_ptr->par_vector_maps.count(name) < 1) {
        std::string errstr = str(boost::format("Could not find: %1%") % name);
        throw std::invalid_argument(errstr);
    }
    return *(d_ptr->par_vector_maps.at(name));
}

template <>
void CommonSystemImpl::setPar(char* name, ParVectorMap value) {
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


/////////////////////////////////////
//
//       Subsystem handling
//
////////////////////////////////////
void CommonSystemImpl::__preSim()
{
    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        sys->__preSim();
    }

    this->preSim();
}

void CommonSystemImpl::__preStep()
{

	this->preStep();
	this->__copyinputs();
	this->__copyoutputs();
    this->__copystateoutputs();

    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        sys->__preStep();
    }

}

void CommonSystemImpl::__doStep(double time)
{

    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        sys->__doStep(time);
    }

    this->doStep(time);
    this->__copyoutputs();
}

void CommonSystemImpl::__postStep()
{
    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        sys->__postStep();
    }

    this->postStep();
    this->__copystateoutputs();
    this->__copyoutputs();
}


////////////////////////////////////
//
//       Connections
//
////////////////////////////////////

void CommonSystemImpl::__copyinputs() {
    this->copyinputs();
    connectionHandler.copyinputs();
}

void CommonSystemImpl::__copyoutputs() {
    this->copyoutputs();
    connectionHandler.copyoutputs();
}

void CommonSystemImpl::__copystateoutputs() {
    this->copystateoutputs();
    connectionHandler.copystateoutputs();
}


std::vector<double*> CommonSystemImpl::getStatePointers() {
    std::vector<double*> out;
    boost::range::push_back(out, states.getPointers());

    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        boost::range::push_back(out, sys->getStatePointers());
    }
    return out;
}

std::vector<double*> CommonSystemImpl::getDerPointers() {
    //In this function we iterate over the state scalars to
    //get the ders in the same order as the states.
    std::vector<double*> out;
    boost::range::push_back(out, ders.getPointers());

    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        boost::range::push_back(out, sys->getDerPointers());
    }
    return out;
}

void CommonSystemImpl::doStoreStep(double time) {
    d_ptr->storeHandler.doStoreStep(time);

    // Subsystems
    for (auto const &sys : d_ptr->subsystems_vec) {
        sys->doStoreStep(time);
    }
}



//Put the state, der, input or output named "name" in the vector of pointers
//to be stored. If none with "name" is found the function raises an invalid_argument
//exception.
void  CommonSystemImpl::store(const char* name) {
    StoreHandler* shp = &d_ptr->storeHandler;
    if (shp->checkAndStore(name, outputs)) return;
    if (shp->checkAndStore(name, states)) return;
    if (shp->checkAndStore(name, ders)) return;
    if (shp->checkAndStore(name, inputs)) return;

    char errmsg[50];
    snprintf(errmsg, sizeof(errmsg), "Could not store: %s, no such variable", name);
    throw std::invalid_argument(errmsg);

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

void CommonSystemImpl::add_compare_greater(char* comparename, double comparevalue, unsigned int col) {
	using std::make_pair;

	if (outputs.d_ptr->vectors.count(comparename) == 1) {
		auto p = make_pair(&(outputs.d_ptr->vectors[comparename]->coeffRef(col)), comparevalue);
		d_ptr->compare_greater_vector.push_back(p);
	}
	else if (states.d_ptr->vectors.count(comparename) == 1) {
		auto p = make_pair(&(states.d_ptr->vectors[comparename]->coeffRef(col)), comparevalue);
		d_ptr->compare_greater_vector.push_back(p);
	}
	else {
		std::string errtxt("Could not find state or output to use for comparison");
		throw std::invalid_argument(errtxt);
	}
}

void CommonSystemImpl::add_compare_greater(char* comparename, double comparevalue, unsigned int row, unsigned int col) {
	using std::make_pair;

	if (outputs.d_ptr->matrices.count(comparename) == 1) {
		auto p = make_pair(&(outputs.d_ptr->matrices[comparename]->coeffRef(row, col)), comparevalue);
		d_ptr->compare_greater_vector.push_back(p);
	}
	else if (states.d_ptr->matrices.count(comparename) == 1) {
		auto p = make_pair(&(states.d_ptr->matrices[comparename]->coeffRef(row)), comparevalue);
		d_ptr->compare_greater_vector.push_back(p);
	}
	else {
		std::string errtxt("Could not find state or output to use for comparison");
		throw std::invalid_argument(errtxt);
	}
}

void CommonSystemImpl::add_compare_smaller(char* comparename, double comparevalue) {
	using std::make_pair;

	if (outputs.d_ptr->scalars.count(comparename) == 1) {
		auto p = make_pair(outputs.d_ptr->scalars[comparename], comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else if (states.d_ptr->scalars.count(comparename) == 1) {
		auto p = make_pair(states.d_ptr->scalars[comparename], comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else {
		std::string errtxt("Could not find state or output to use for comparison");
		throw std::invalid_argument(errtxt);
	}
}

void CommonSystemImpl::add_compare_smaller(char* comparename, double comparevalue, unsigned int col) {
	using std::make_pair;

	if (outputs.d_ptr->vectors.count(comparename) == 1) {
		auto p = make_pair(&(outputs.d_ptr->vectors[comparename]->coeffRef(col)), comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else if (states.d_ptr->vectors.count(comparename) == 1) {
		auto p = make_pair(&(states.d_ptr->vectors[comparename]->coeffRef(col)), comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else {
		std::string errtxt("Could not find state or output to use for comparison");
		throw std::invalid_argument(errtxt);
	}
}

void CommonSystemImpl::add_compare_smaller(char* comparename, double comparevalue, unsigned int row, unsigned int col) {
	using std::make_pair;

	if (outputs.d_ptr->matrices.count(comparename) == 1) {
		auto p = make_pair(&(outputs.d_ptr->matrices[comparename]->coeffRef(row, col)), comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else if (states.d_ptr->matrices.count(comparename) == 1) {
		auto p = make_pair(&(states.d_ptr->matrices[comparename]->coeffRef(row, col)), comparevalue);
		d_ptr->compare_smaller_vector.push_back(p);
	}
	else {
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

    // Subsystems
    bool subsystem_triggered = false;
    for (auto const &sys : d_ptr->subsystems_vec) {
        subsystem_triggered = subsystem_triggered || sys->do_comparison();
    }

    return is_greater || is_smaller || subsystem_triggered;
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

void CommonSystemImpl::add_subsystem(SimulatableSystemInterface * subsystem, string name)
{
    if (subsystem->getDiscrete()) {
        throw std::invalid_argument("Discrete systems not supported as subsystems");
    }
    if (d_ptr->subsystems.count(name) > 0) {
        throw std::invalid_argument("A subsystem with this name already exists!");
    }
    subsystem_names.push_back(name);
    d_ptr->subsystems[name] = subsystem;
    d_ptr->subsystems_vec.push_back(subsystem);
}

SimulatableSystemInterface * CommonSystemImpl::get_subsystem(std::string name)
{
    if (d_ptr->subsystems.count(name) > 0) {
        return d_ptr->subsystems[name];
    }
    else {
        throw std::invalid_argument("A subsystem with this name doesn't exist!");
    }
}

}