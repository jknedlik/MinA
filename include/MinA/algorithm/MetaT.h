#include "MinA/common/algorithm.hpp"
#include "MinA/common/utility.hpp"
#include <sstream>
#include <stdlib.h>
// template <size_t numA, typename Algo, typename Func>
namespace MinA {
template <typename Algo>

using mptype = decltype(Algo().getMetaParameters());
template <typename Algo>
class F : public Function<mptype<Algo>> {

 public:
 using Basetype = Function<mptype<Algo>>;
 using result =
   MinA::Result<mptype<Algo>, typename decltype(Algo::f)::parametertype>;
 Algo alg;
 size_t index;
 result rs;
 F() : Function<mptype<Algo>>::Function(), alg()
 {
  this->bounds = alg.getMetaBoundaries();
  this->startvalues = alg.getMetaParameters();
  index = 0;
  this->fn = alg.getFileName();
 };
 result operator()(mptype<Algo> mp)
 {
  alg.reset();
  alg.setFileName(this->fn + std::to_string(index));
  alg.setMetaParameters(mp);
  index++;

  auto res = alg.run();
  rs.parameters = mp;
  rs.informations = res.parameters;
  rs.value = res.value;
  return rs;
 }
};
} // namespace MinA
