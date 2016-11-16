#ifndef FUNCTIONTOBEOPTIMIZED_H
#define FUNCTIONTOBEOPTIMIZED_H
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include "MinA/common/Parameter.h"
#include <algorithm>
#include <iterator>
#include <memory>
#include <sstream>
#include "MinA/common/Exception.h"
#include "MinA/common/Log.h"

enum handle_arith { MinA_EHIGH, MinA_ETHROW, MinA_EWARN };
class FunctionToBeOptimized {
  private:
    handle_arith handle;
    long unsigned int mDimensions;

  public:
    FunctionToBeOptimized() = delete;

    FunctionToBeOptimized(unsigned int dim) : handle{ MinA_ETHROW }, mDimensions{ dim } {}

    FunctionToBeOptimized(std::vector<Parameter> pars)
      : handle{ MinA_ETHROW }, mDimensions{ pars.size() }, mParameters{ pars }
    {
    }

    FunctionToBeOptimized(std::vector<std::string> parNames,
                          std::vector<double> startingValues, std::vector<double> leftBoundaries,
                          std::vector<double> rightBoundaries)
      : FunctionToBeOptimized(parNames.size())
    {
        if (leftBoundaries.size() != mDimensions || rightBoundaries.size() != mDimensions ||
            startingValues.size() != mDimensions) {
            throw std::logic_error("Argument vector sizes differ.");
        }

        Parameter par;
        for (int i = 0; i < mDimensions; ++i) {
            par.setName(parNames[i]);
            par.setStartingValue(startingValues[i]);
            par.setLeftBoundary(leftBoundaries[i]);
            par.setRightBoundary(rightBoundaries[i]);
            mParameters.push_back(par);
        }
    }

    FunctionToBeOptimized(std::vector<std::string> parNames, double startingValue,
                          double leftBoundary, double rightBoundary)
      : FunctionToBeOptimized(parNames.size())
    {
        Parameter par;
        for (int i = 0; i < mDimensions; ++i) {
            par.setName(parNames[i]);
            par.setStartingValue(startingValue);
            par.setLeftBoundary(leftBoundary);
            par.setRightBoundary(rightBoundary);
            mParameters.push_back(par);
        }
    }

    void setHandle(handle_arith ha) { handle = ha; }

    double evaluate(std::vector<double> para)
    {
        double result;
        try {
            result = getEvaluation(para);
            if (!std::isfinite(result))
                throw Arithmetical_Exception(result);
        }
        catch (Arithmetical_Exception& ae) {
            std::stringstream err;
            switch (handle) {
                case MinA_EHIGH:
                    err << "Exception caught:" << ae.what()
                        << ". Returning very high Value (10E+7) \n";
                    Log::getLog() << err.str();
                    Log::getLog().flushLog();
                    result = 10000000.0;
                    break;
                case MinA_ETHROW:
                    err << "Exception caught:" << ae.what() << ". Rethrowing Exception";
                    throw;
                    break;
                case MinA_EWARN:
                    err << "Exception caught:" << ae.what() << ". Printing out this warning";
                    result = ae.getDoubleValue();
                    break;
            }
        }
        return result;
    }

    //virtual double getEvaluation(std::map<std::string, double> para) = 0;
    virtual double getEvaluation(std::vector<double> params) = 0;

    int getParSpaceDim()
    {
        // return parameters.size();
        return mDimensions;
    }

    std::vector<Parameter> mParameters;
};

#endif
