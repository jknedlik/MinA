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
	
	 void setOptimizationAlgorithmParameter(string nam,double nu);
	 double getOptimizationAlgorithmParameter(string nam);
	virtual void save()const=0;
	virtual void restore()=0;
	std::map <string, double> getAdditionalInformation()const{return additionalInformation;}
	void setAdditionalInformation(std::map <string, double> additionalInformation_re){additionalInformation=additionalInformation_re;}
    private:
	std::map <string, double> optimizationAlgorithmParameter;
	std::map <string, double> additionalInformation;
};

#endif
