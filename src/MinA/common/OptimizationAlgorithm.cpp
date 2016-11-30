#include "MinA/common/OptimizationAlgorithm.h"
using namespace std;

OptimizationAlgorithm::~OptimizationAlgorithm() {}

void OptimizationAlgorithm::setMetaParameter(string parName, double parValue)
{
    mMetaParameters[parName] = parValue;
}

double OptimizationAlgorithm::getMetaParameter(string parName) { return mMetaParameters[parName]; }

map<string, string> OptimizationAlgorithm::getAdditionalInformation() const
{
    return mAdditionalInformation;
}

void OptimizationAlgorithm::setAdditionalInformation(map<string, string> additionalInformation)
{
    mAdditionalInformation = additionalInformation;
}

void OptimizationAlgorithm::setAdditionalInformation(string s1, string s2)
{
    mAdditionalInformation[s1] = s2;
}
