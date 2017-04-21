#ifndef MYFUNCTION_CPP
#include <cmath>
#include <iostream>
#include <map>
#define PI 3.14159265358979323846
#include "MinA/common/FunctionToBeOptimized.h"
#include <stdexcept>
#include <string>
#include <vector>
#define MYFUNCTION_CPP
using namespace std;

class McCormick_function : public FunctionToBeOptimized {
 public:
 McCormick_function() : FunctionToBeOptimized(2) {}
 McCormick_function(int dim) : FunctionToBeOptimized(2)
 {
  if (dim != 2)
   throw std::logic_error("Argument vector sizes differ.");
 }
 McCormick_function(vector<Parameter> pars) : FunctionToBeOptimized(pars) {}
 McCormick_function(vector<string> parNames, std::vector<double> startingValues,
                    std::vector<double> leftBoundaries,
                    std::vector<double> rightBoundaries)
   : FunctionToBeOptimized(parNames, startingValues, leftBoundaries,
                           rightBoundaries)
 {
 }

 double getEvaluation(vector<double> params)
 {
  double fz;
  fz = sin(params[0] + params[1]) + pow(params[0] - params[1], 2) -
       1.5 * params[0] + 2.5 * params[1] + 1;
  return fz;
 };
};

#endif
