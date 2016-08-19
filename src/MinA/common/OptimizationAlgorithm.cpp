#include "MinA/common/OptimizationAlgorithm.h"

OptimizationAlgorithm::~OptimizationAlgorithm(){;}
void OptimizationAlgorithm::SetOptimizationAlgorithmParameter(string nam,double nu){
		OptimizationAlgorithmParameter[nam]=nu;
	};
double OptimizationAlgorithm::GetOptimizationAlgorithmParameter(string nam){
		return OptimizationAlgorithmParameter[nam];
	};
