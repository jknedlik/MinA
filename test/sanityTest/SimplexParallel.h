#ifndef SIMPLEXPARALLEL_H
#define SIMPLEXPARALLEL_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"
#include "Simplex.h"


class SimplexParallel:public OptimizationAlgorithm{
    public:
	SimplexParallel();
	~SimplexParallel();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(vertex *para);
    protected:
	int dimension;

  
};

#endif
