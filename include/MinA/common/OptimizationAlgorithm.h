#ifndef OPTIMiZATIONALGORITHM_H
#define OPTIMiZATIONALGORITHM_H
#include <map>
#include <iostream>
#include<memory>
using namespace std;
#include"MinA/common/Functiontobeoptimized.h"
#include"MinA/common/Result.h"
 using namespace std;
class OptimizationAlgorithm{
    public:
	virtual ~OptimizationAlgorithm();
	virtual Result algorithm(Functiontobeoptimized *start)=0;
	
	 void SetOptimizationAlgorithmParameter(string nam,double nu);
	 double GetOptimizationAlgorithmParameter(string nam);
    private:
	std::map <string, double> OptimizationAlgorithmParameter;
	std::map <string, double> additionalIntregration;
};

#endif
