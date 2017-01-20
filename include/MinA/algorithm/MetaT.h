#include "MinA/common/algorithm.hpp"
#include <sstream>
#include <stdlib.h>
// template <size_t numA, typename Algo, typename Func>
namespace MinA {
template <typename Algo>
class F : public Function<decltype(Algo::mMetaParameters)> {
  public:
    Algo alg;
    size_t index;
    std::string origfile;
    Result<typename decltype(Algo::f)::parametertype> rs;
    F() : Function<decltype(Algo::mMetaParameters)>::Function(), alg()
    {
        this->bounds = alg.mMetaBoundaries;
        this->startvalues = alg.mMetaParameters;
        index = 0;
        origfile = alg.filename;
    };
    double evaluate(decltype(alg.mMetaParameters) mp)
    {
        alg.reset();
        alg.filename = origfile + std::to_string(index);
        alg.mMetaParameters = mp;
        index++;
        rs = alg.run();
        return rs.value;
    }
};
}
