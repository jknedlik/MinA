#ifndef SIMPLEXPARALLEL_H
#define SIMPLEXPARALLEL_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"

struct vertex{
  		std::map <std::string, double> par;
    		double func;
	};

class SimplexParallel:public OptimizationAlgorithm{
    public:
	SimplexParallel();
	~SimplexParallel();
	Result algorithm(Functiontobeoptimized* start);
	void showVertex(vertex *para);
	void showM(vertex &para);
	void sendVertex(vertex &A,int receiver,int tag);
	vertex receiveVertex(int sender,int tag);
	
    protected:
	int dimension;
	Functiontobeoptimized* function;
	
	void calculateM(vertex *A,vertex &M,int world_size);
	void calculateAr(vertex &Ar,vertex &M,vertex &Aj);
	void calculateAe(vertex &Ae,vertex &M,vertex &Ar);
	void calculateAc(vertex &Ac,vertex &M,vertex &Ajp);
	void push(vertex &a,vertex &b);
	void pushResult(Result &rs,vertex &A);
	void newVertex(vertex *A);
	void InitialVertex(vertex *A);
	
  
};

#endif
