#include "MinA/common/OptimizationAlgorithm.h"

OptimizationAlgorithm::~OptimizationAlgorithm(){;}
void OptimizationAlgorithm::setOptimizationAlgorithmParameter(string nam,double nu){
		optimizationAlgorithmParameter[nam]=nu;
	};
double OptimizationAlgorithm::getOptimizationAlgorithmParameter(string nam){
		return optimizationAlgorithmParameter[nam];
	};
