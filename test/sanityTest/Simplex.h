#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"

struct vertex{
  		std::map <std::string, double> par;
    		double func;
	};

class Simplex:public OptimizationAlgorithm{
    public:
	Simplex();
	~Simplex();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(vertex *para);
   protected:
	int dimension;
  
};

#endif
