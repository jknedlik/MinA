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
   private:
  	void save()const;
	void restore();
	vertexVector Acopy;
};

#endif
