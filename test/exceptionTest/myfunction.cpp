#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
#include "MinA/common/FunctionToBeOptimized.h"

#define PI 3.14159265358979323846
#define MYFUNCTION_CPP
using namespace std;

class BadTen : public FunctionToBeOptimized {
    int mCounter;

  public:
    // BadTen() = delete;

    BadTen(int dim = 3) : FunctionToBeOptimized(dim), mCounter(0)
    {
        Parameter first("x", 3);
        Parameter second("y", 4);
        Parameter third("z", 5);

        mParameters.push_back(first);
        mParameters.push_back(second);
        mParameters.push_back(third);
    };

    double getEvaluation(vector<double> params)
    {
        mCounter++;
        if (!(mCounter % 10))
            return (1.0 / 0.0);
        return pow(params[0], 2) + pow(params[1], 2) + pow(params[2], 2);
    }
};

#endif
