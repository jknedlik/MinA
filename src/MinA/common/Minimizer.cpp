#include "MinA/common/Minimizer.h"

Result Minimizer::minimize(Functiontobeoptimized *start,OptimizationAlgorithm *sim ){
	return sim->algorithm(start);
	}
Result Minimizer::operator()(Functiontobeoptimized *start,OptimizationAlgorithm *sim){
	return minimize(start,sim); 
	}
