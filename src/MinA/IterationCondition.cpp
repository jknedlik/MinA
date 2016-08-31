#include "MinA/common/IterationCondition.h"

bool IterationCondition::check(std::map<string,string>info){
	return (std::stoi(info["currentiteration"]) <std::stoi(info["iterations"]) ;
	}
