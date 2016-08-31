#ifndef ITERATIONCONDITION_H
#define ITERATIONCONDITION_H
#include <iostream>
#include "MinA/common/StopingCondition.h"

class IterationCondition:public StopingCondition{
bool check(std::map<string,string>info);
};


#endif


