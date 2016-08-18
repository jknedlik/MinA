#ifndef FUNCTIONTOBEOPTIMIZED_H	
#define FUNCTIONTOBEOPTIMIZED_H

#include <map>
#include <iostream>
#include <set>
#include "MinA/common/Parameter.h"
 using namespace std;
class Functiontobeoptimized{
    public:
	virtual double evaluate(std::map <string, double> para)=0;
	int getparametersize(){return parameters.size();}
	std::set<Parameter> parameters;
    protected:
	
	
};

#endif 
