#pragma once
#include "MinA/common/OptimizationAlgorithm.h"
#include "MinA/common/Log.h"
#include <tuple>

template<class Algorithm>

typedef std::tuple<std::string,double,double,double> OAParameter
	//name, lboundary,rboundary,stepsize
class MetaOptimizer:OptimizationAlgorithm{
private:
	MetaOptimizer(int parallelAlgoNum=10);
	std::vector<Algorithm>;
	std::vector<OAParameterList>;
public:
	addOAParameterList(OAParameter);
	Result algorithm(std::shared_ptr<FunctionToBeOptimized>start);
	
};
