#ifndef OPTIMiZATIONALGORITHM_H
#define OPTIMiZATIONALGORITHM_H
#include <map>
#include <iostream>
#include <memory>
#include "MinA/common/FunctionToBeOptimized.h"
#include "MinA/common/Result.h"

class OptimizationAlgorithm {
  public:
    virtual ~OptimizationAlgorithm();
    virtual Result algorithm(std::shared_ptr<FunctionToBeOptimized> start) = 0;

    void setMetaParameter(string parName, double parValue);
    double getMetaParameter(string parName);
    virtual void save() const = 0;
    virtual void restore() = 0;
    std::map<string, string> getAdditionalInformation() const;
    void setAdditionalInformation(std::map<string, string> additionalInformation);
    void setAdditionalInformation(string s1, string s2);

  private:
    std::map<string, double> mMetaParameters;
    std::map<string, string> mAdditionalInformation;
};

#endif
