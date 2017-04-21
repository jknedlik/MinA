#include "MinA/common/algorithm.hpp"
#include <sstream>
#include <stdlib.h>
// template <size_t numA, typename Algo, typename Func>
namespace MinA {
template <typename Algo>

using mptype = decltype(Algo().getMetaParameters());
template <typename Algo>
class F : public Function<mptype<Algo>> {
 public:
 Algo alg;
 size_t index;
 Result<typename decltype(Algo::f)::parametertype> rs;
 F() : Function<mptype<Algo>>::Function(), alg()
 {
  this->bounds = alg.getMetaBoundaries();
  this->startvalues = alg.getMetaParameters();
  index = 0;
  this->fn = alg.getFileName();
 };
 double evaluate(mptype<Algo> mp)
 {
  alg.reset();
  alg.setFileName(this->fn + std::to_string(index));
  alg.setMetaParameters(mp);
  index++;
  rs = alg.run();
  return rs.value;
 }
};
}
