#ifndef SIMPLEX_H
#define SIMPLEX_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include "MinA/common/OptimizationAlgorithm.h"
#include "MinA/common/Log.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>

typedef std::pair <std::map <std::string, double>,double> vertex;
typedef std::vector<std::pair <std::map <std::string, double>,double>> vertexVector;
class Simplex:public OptimizationAlgorithm{
    public:
	Simplex();
	~Simplex();
	Result algorithm(Functiontobeoptimized* start);
	void showVertex(vertexVector &para);
	void showVertex(vertex &para);
   protected:
	int dimension;
	Functiontobeoptimized* function;
	bool checkStopingCondition();
	void push(vertex &a,vertex &b);
	void pushResult(Result &rs,vertex &A);
	void InitialVertex(vertexVector &A);
	void calculateM(vertexVector &A,vertex &M,int world_size);
	void calculateAr(vertex &Ar,vertex &M,vertex &Aj);
	void calculateAe(vertex &Ae,vertex &M,vertex &Ar);
	void calculateAc(vertex &Ac,vertex &M,vertex &Ajp);	
	void newVertex(vertexVector &A);
	void save()const;
	void restore();
	vertexVector Acopy;
   private:
  	
};

#endif
