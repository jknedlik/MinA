#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"


typedef std::pair <std::map <std::string, double>,double> vertex;
typedef std::vector<std::pair <std::map <std::string, double>,double>> vertexVector;
class Simplex:public OptimizationAlgorithm{
    public:
	Simplex();
	~Simplex();
	Result algorithm(Functiontobeoptimized* start);
	void showfunc(std::vector<vertex> &para);
   protected:
	int dimension;
  
};

#endif
