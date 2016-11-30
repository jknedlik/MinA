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

    void setMetaParameter(std::string parName, double parValue);
    double getMetaParameter(std::string parName);
    virtual void save() const = 0;
    virtual void restore() = 0;
    std::map<std::string, std::string> getAdditionalInformation() const;
    void setAdditionalInformation(std::map<std::string, std::string> additionalInformation);
    void setAdditionalInformation(std::string s1, std::string s2);

  private:
    std::map<std::string, double> mMetaParameters;
    std::map<std::string, std::string> mAdditionalInformation;
};

#endif
