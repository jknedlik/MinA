#ifndef SIMPLEXPARALLEL_H
#define SIMPLEXPARALLEL_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"


class SimplexParallel:public OptimizationAlgorithm{
    public:
	SimplexParallel();
	~SimplexParallel();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(Functiontobeoptimized* start,std::map <string, double> *para);

  
};

#endif
