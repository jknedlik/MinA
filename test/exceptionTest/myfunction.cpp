#ifndef MYFUNCTION_CPP
#include "MinA/common/FunctionToBeOptimized.h"
#include <cmath>
#define MYFUNCTION_CPP
using namespace std;

class BadTen : public FunctionToBeOptimized {
 int mCounter;

 public:
 BadTen(vector<Parameter> pars) : FunctionToBeOptimized(pars) {}
 // if (pars.size() != 3) throw

 double getEvaluation(vector<double> params)
 {
  mCounter++;
  if (!(mCounter % 10))
   return (1.0 / 0.0);
  return pow(params[0], 2) + pow(params[1], 2) + pow(params[2], 2);
 }
};

#endif
