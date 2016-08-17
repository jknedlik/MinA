#ifndef FUNCTIONTOBEOPTIMIZED_H	
#define FUNCTIONTOBEOPTIMIZED_H

#include <map>
#include <iostream>
#include <set>
 using namespace std;
class Functiontobeoptimized{
    public:
	virtual double evaluate(std::map <string, double> para)=0;
	int getparametersize(){return parameters.size();}
	std::set<std::string> parameters;
    protected:
	
	
};

#endif 
