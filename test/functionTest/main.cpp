//#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/utility.hpp"

//#include "MinA/algorithm/SimplexParallel.h"
//#include "MinA/common/Minimizer.h"
//#include "MinA/common/Result.h"
//#include "TestFunction.cpp"
#include "mpi.h"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <tuple>
#include <utility>
using namespace std;

template <typename ps>
class BaseF {
 public:
 using Basetype = BaseF<ps>;
 using parametertype = ps;
 using Parameters = ps;
 using boundarytype = typename Boundarytuple<ps>::type;
 using result = Result<parametertype, void>;
 BaseF() : mpi_procs(1), fn(".Function"){};
 BaseF(parametertype start, boundarytype bs)
   : startvalues(start), bounds(bs), mpi_procs(1), fn(".Function"){};

 protected:
 boundarytype bounds;
 parametertype startvalues;
 std::string fn;
 size_t mpi_procs;
};
using Basetype = Function<std::tuple<int, int>>;
class TestF : public Function<std::tuple<int, int>> {
 public:
 // using result = Result<parametertype, std::tuple<int>>;
 TestF() : Function({ 5, 3 }, { { 0, 10 }, { 0, 10 } }) { ; }
 result operator()(Parameters a)
 {
  result b;
  b.value = std::get<0>(a);
  return b;
 }
};

int main() {}
