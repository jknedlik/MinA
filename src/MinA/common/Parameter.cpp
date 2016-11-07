#include "MinA/common/Parameter.h"
#include <cstring>
using namespace std;

bool Parameter::operator<(const Parameter& e) const
{
    return strcmp(getName().c_str(), e.getName().c_str()) < 0;
}
