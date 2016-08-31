#ifndef SIMPLEXPARALLEL_H
#define SIMPLEXPARALLEL_H
#include <map>
#include <iostream>
#include<memory>
#include "mpi.h"
#include <boost/serialization/vector.hpp>
#include "MinA/common/OptimizationAlgorithm.h"
#include <boost/serialization/utility.hpp>
#include <boost/serialization/map.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include "Simplex.h"


typedef std::pair <std::map <std::string, double>,double> vertex;
typedef std::vector<std::pair <std::map <std::string, double>,double>> vertexVector;

class SimplexParallel:public Simplex{
    public:
	SimplexParallel();
	~SimplexParallel();
	Result algorithm(Functiontobeoptimized* start);
	//void showVertex(vertexVector &para);
	//void showVertex(vertex &para);
	void sendVertex(vertex &A,int receiver,int tag);
	vertex receiveVertex(int sender,int tag);
	
	
    protected:
	int dimension;
	Functiontobeoptimized* function;
	
	/*void calculateM(vertexVector &A,vertex &M,int world_size);
	void calculateAr(vertex &Ar,vertex &M,vertex &Aj);
	void calculateAe(vertex &Ae,vertex &M,vertex &Ar);
	void calculateAc(vertex &Ac,vertex &M,vertex &Ajp);
	void push(vertex &a,vertex &b);
	void pushResult(Result &rs,vertex &A);
	void newVertex(vertexVector &A);
	void InitialVertex(vertexVector &A);
     private:
  	void save()const;
	void restore();
	vertexVector Acopy;
	/*/
  
};

#endif
