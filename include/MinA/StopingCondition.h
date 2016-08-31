#ifndef STOPINGCONDITION_H
#define STOPINGCONDITION_H
#include <iostream>

class StoppingCondition{
virtual bool check(std::map<string,string>info)=0; 
};


#endif
