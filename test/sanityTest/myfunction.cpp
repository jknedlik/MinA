#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>


#define MYFUNCTION_CPP
#include "MinA/common/Functiontobeoptimized.h"

class Myfunction:public Functiontobeoptimized{
    public:
	Myfunction(){
		Parameter first,second;
		first.setname("x");
		second.setname("y");	
		parameters.insert(first);
		parameters.insert(second);
		//parameters.insert("x");
		//parameters.insert("y");
	};

	double evaluate(std::map <string, double> para){
	//return para["x"]*para["x"]+para["y"]*para["y"];
	return (para["x"]*para["x"]+para["y"]-11)*(para["x"]*para["x"]+para["y"]-11)+(para["x"]+para["y"]*para["y"]-7)*(para["x"]+para["y"]*para["y"]-7);
	}
 
};
#endif 
