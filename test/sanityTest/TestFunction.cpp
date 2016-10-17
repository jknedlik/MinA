#ifndef MYFUNCTION_CPP
#include <iostream>
#include <map>
#include <cmath>
#define PI 3.14159265358979323846
#include "MinA/common/FunctionToBeOptimized.h"
#include <string>
#define MYFUNCTION_CPP
using namespace std;

class SquareFunction : public FunctionToBeOptimized {
  public:
    SquareFunction(int dimension)
    {
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingPoint(8);
            par.setBoundaryLeft(-10);
            par.setBoundaryRight(10);
            parameters.insert(par);
        }
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_square = 0;
        for (auto it : parameters)
            sum_square += para[it.getName()] * para[it.getName()];
        return sum_square;
    }
};

class Himmelblaufunction : public FunctionToBeOptimized {
  public:
    Himmelblaufunction()
    {
        Parameter first, second;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(map<string, double> para)
    {

        return (para["x"] * para["x"] + para["y"] - 11) * (para["x"] * para["x"] + para["y"] - 11) +
               (para["x"] + para["y"] * para["y"] - 7) * (para["x"] + para["y"] * para["y"] - 7);
    }
};

class Boot_s_function : public FunctionToBeOptimized {
  public:
    Boot_s_function()
    {
        Parameter first, second;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(map<string, double> para)
    {

        return pow((para["x"] + 2 * para["y"] - 7), 2) + pow((2 * para["x"] + para["y"] - 5), 2);
    }
};

class Michalewicz_function : public FunctionToBeOptimized {
  public:
    Michalewicz_function()
    {
        Parameter first, second, third;
        first.setName("x");
        first.setStartingPoint(3);
        second.setName("y");
        second.setStartingPoint(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_xs = 0;
        double fz = 0;
        for (auto it : parameters)
            sum_xs += pow(para[it.getName()], 2);
        fz = -sin(para["x"]) * pow(sin(pow(para["x"], 2) / PI), 2) -
             sin(para["x"]) * pow(sin(2 * pow(para["y"], 2) / PI), 2);
        return fz;
    }
};

class Matthias_function : public FunctionToBeOptimized {
  public:
    Matthias_function(int dimension)
    {
        double alpha = 6;
        double beta = 3;
        double gamma = 1;
        double eta = 1;
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingPoint(i * 0.5);
            par.setBoundaryLeft(-5);
            par.setBoundaryRight(5);
            parameters.insert(par);
        }
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_xs = 0;
        for (auto it : parameters)
            sum_xs += pow(para[it.getName()], 2);
        double fz = alpha * pow(sin(beta * sum_xs), 2) + sum_xs * gamma * exp(eta * sum_xs);
        return fz;
    }
};

class McCormick_function : public FunctionToBeOptimized {
  public:
    McCormick_function()
    {
        Parameter first, second, third;
        first.setName("x");
        first.setStartingPoint(0);
        first.setBoundaryLeft(-1.5);
        first.setBoundaryRight(4);
        second.setName("y");
        second.setStartingPoint(0);
        second.setBoundaryLeft(-3);
        second.setBoundaryRight(4);
        parameters.insert(first);
        parameters.insert(second);
    };

    double getEvaluation(map<string, double> para)
    {
        double fz;
        fz = sin(para["x"] + para["y"]) + pow(para["x"] - para["y"], 2) - 1.5 * para["x"] +
             2.5 * para["y"] + 1;
        return fz;
    };
};

class Schwefel_function : public FunctionToBeOptimized {
  public:
    Schwefel_function(int dimension)
    {
        d = dimension;
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingPoint(50);
            par.setBoundaryLeft(-500);
            par.setBoundaryRight(500);
            parameters.insert(par);
        }
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_xsin = 0;
        for (auto it : parameters)
            sum_xsin += para[it.getName()] * sin(sqrt(abs(para[it.getName()])));
        double fz = 418.9829 * d - sum_xsin;
        return fz;
    }
    double d;
};

class Gaussian_function : public FunctionToBeOptimized {
  public:
    Gaussian_function(int dimension)
    {
        d = dimension;
        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingPoint(5);
            par.setBoundaryLeft(-10);
            par.setBoundaryRight(10);
            parameters.insert(par);
        }
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_xsin = 0;
        for (auto it : parameters)
            sum_xsin += pow((para[it.getName()] + 1), 2);
        double fz = -exp(-sum_xsin);
        return fz;
    }
    double d;
};

class Modify_Matthias_function : public FunctionToBeOptimized {
  public:
    Modify_Matthias_function(int dimension)
    {
        double alpha = 6;
        double beta = 3;
        double gamma = 1;
        double eta = 1;

        for (int i = 0; i < dimension; i++) {
            Parameter par;
            par.setName("x" + to_string(i));
            par.setStartingPoint(i * 0.5);
            par.setBoundaryLeft(-5);
            par.setBoundaryRight(5);
            parameters.insert(par);
        }
    };

    double getEvaluation(map<string, double> para)
    {
        double sum_xs = 0;
        for (auto it : parameters)
            sum_xs += pow(para[it.getName()], 2);
        double fz = alpha * pow(cos(beta * sum_xs), 2) + gamma * exp(eta * sum_xs);
        return fz;
    }
};
#endif
