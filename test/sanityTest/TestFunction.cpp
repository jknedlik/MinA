#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
#define PI 3.14159265358979323846
#include "MinA/common/FunctionToBeOptimized.h"
#include <vector>
#include <string>
#include <stdexcept>
#define MYFUNCTION_CPP
using namespace std;

class SquareFunction : public FunctionToBeOptimized {
  public:
    SquareFunction(int dimension) : FunctionToBeOptimized(dimension) {}
    SquareFunction(vector<string> parNames, double startingValue, double leftBoundary,
                   double rightBoundary)
      : FunctionToBeOptimized(parNames, startingValue, leftBoundary, rightBoundary)
    {
    }

    double getEvaluation(vector<double> params)
    {
        double sum_square = 0.;
        for (int iPar = 0; iPar < getParSpaceDim(); ++iPar)
            sum_square += pow(params[iPar], 2);
        return sum_square;
    }
};

class Himmelblaufunction : public FunctionToBeOptimized {
  public:
    Himmelblaufunction(vector<string> parNames, std::vector<double> startingValues,
                       std::vector<double> leftBoundaries, std::vector<double> rightBoundaries)
      : FunctionToBeOptimized(parNames, startingValues, leftBoundaries, rightBoundaries)
    {
    }

    double getEvaluation(vector<double> params)
    {
        return (params[0] * params[0] + params[1] - 11) * (params[0] * params[0] + params[1] - 11) +
               (params[0] + params[1] * params[1] - 7) * (params[0] + params[1] * params[1] - 7);
    }
};

class Boot_s_function : public FunctionToBeOptimized {
  public:
    Boot_s_function() : FunctionToBeOptimized(2) {}
    Boot_s_function(int dim) : FunctionToBeOptimized(2)
    {
        if (dim != 2)
            throw std::logic_error("Argument vector sizes differ.");
    }
    Boot_s_function(vector<string> parNames, std::vector<double> startingValues,
                    std::vector<double> leftBoundaries, std::vector<double> rightBoundaries)
      : FunctionToBeOptimized(parNames, startingValues, leftBoundaries, rightBoundaries)
    {
    }

    double getEvaluation(vector<double> params)
    {
        return pow((params[0] + 2 * params[1] - 7), 2) + pow((2 * params[0] + params[1] - 5), 2);
    }
};

class Michalewicz_function : public FunctionToBeOptimized {
  public:
    Michalewicz_function() : FunctionToBeOptimized(2) {}
    Michalewicz_function(int dim) : FunctionToBeOptimized(2)
    {
        if (dim != 2)
            throw std::logic_error("Argument vector sizes differ.");
    }
    Michalewicz_function(vector<string> parNames, std::vector<double> startingValues,
                         std::vector<double> leftBoundaries, std::vector<double> rightBoundaries)
      : FunctionToBeOptimized(parNames, startingValues, leftBoundaries, rightBoundaries)
    {
    }

    double getEvaluation(vector<double> params)
    {
        double sum_xs = 0.;
        double fz = 0.;
        for (auto iPar : params)
            sum_xs += pow(iPar, 2);
        fz = -sin(params[0]) * pow(sin(pow(params[0], 2) / PI), 2) -
             sin(params[0]) * pow(sin(2 * pow(params[1], 2) / PI), 2);
        return fz;
    }
};

class Matthias_function : public FunctionToBeOptimized {
  public:
    Matthias_function() = delete;

    // function's internal mParameters
    double mAlpha;
    double mBeta;
    double mGamma;
    double mEta;

    Matthias_function(unsigned int dimension, double alpha = 6., double beta = 3.,
                      double gamma = 1., double eta = 1.)
      : FunctionToBeOptimized(dimension), mAlpha(alpha), mBeta(beta), mGamma(gamma), mEta(eta)
    {
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingValue(i * 0.5);
            par.setLeftBoundary(-5);
            par.setRightBoundary(5);
            mParameters.push_back(par);
        }
    };

    // double getEvaluation(map<string, double> para)
    double getEvaluation(vector<double> params)
    {
        double sum_xs = 0;
        for (auto iPar : params)
            sum_xs += pow(iPar, 2);
        double fz = mAlpha * pow(sin(mBeta * sum_xs), 2) + sum_xs * mGamma * exp(mEta * sum_xs);
        return fz;
    }
};

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
                       std::vector<double> leftBoundaries, std::vector<double> rightBoundaries)
      : FunctionToBeOptimized(parNames, startingValues, leftBoundaries, rightBoundaries)
    {
    }

    double getEvaluation(vector<double> params)
    {
        double fz;
        fz = sin(params[0] + params[1]) + pow(params[0] - params[1], 2) - 1.5 * params[0] +
             2.5 * params[1] + 1;
        return fz;
    };
};

class Schwefel_function : public FunctionToBeOptimized {
  public:
    Schwefel_function(int dimension) : FunctionToBeOptimized(dimension) {}
    Schwefel_function(vector<string> parNames, double startingValue, double leftBoundary,
                      double rightBoundary)
      : FunctionToBeOptimized(parNames, startingValue, leftBoundary, rightBoundary)
    {
    }

    double getEvaluation(vector<double> params)
    {
        double sum_xsin = 0;
        for (auto iPar : params)
            sum_xsin += iPar * sin(sqrt(abs(iPar)));
        double fz = 418.9829 * d - sum_xsin;
        return fz;
    }
    double d;
};

class Gaussian_function : public FunctionToBeOptimized {
  public:
    Gaussian_function(int dimension) : FunctionToBeOptimized(dimension) {}
    Gaussian_function(vector<string> parNames, double startingValue, double leftBoundary,
                      double rightBoundary)
      : FunctionToBeOptimized(parNames, startingValue, leftBoundary, rightBoundary)
    {
    }

    double getEvaluation(vector<double> params)
    {
        double sum_xsin = 0;
        for (auto iPar : params)
            sum_xsin += pow(iPar + 1, 2);
        double fz = -exp(-sum_xsin);
        return fz;
    }
};

class Modified_Matthias_function : public FunctionToBeOptimized {
  public:
    // function's internal mParameters
    double mAlpha;
    double mBeta;
    double mGamma;
    double mEta;

    Modified_Matthias_function(unsigned int dimension, double alpha = 6., double beta = 3.,
                               double gamma = 1., double eta = 1.)
      : FunctionToBeOptimized(dimension), mAlpha(alpha), mBeta(beta), mGamma(gamma), mEta(eta)
    {
        if (dimension < 0)
            for (int i = 0; i < dimension; i++) {
                Parameter par;
                par.setName("x" + to_string(i));
                par.setStartingValue(i * 0.5);
                par.setLeftBoundary(-5);
                par.setRightBoundary(5);
                mParameters.push_back(par);
            }
    };

    double getEvaluation(vector<double> params)
    {
        double sum_xs = 0;
        for (auto iPar : params)
            sum_xs += pow(iPar, 2);
        double fz = mAlpha * pow(cos(mBeta * sum_xs), 2) + mGamma * exp(mEta * sum_xs);
        return fz;
    }
};
#endif
