#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include<memory>
#include "MinA/common/OptimizationAlgorithm.h"


class Simplex:public OptimizationAlgorithm{
    public:
	Simplex();
	~Simplex();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(Functiontobeoptimized* start,std::map <string, double> *para);

  
};

#endif
