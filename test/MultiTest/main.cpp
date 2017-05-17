#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

int main(int argc, char** argv)
{
 std::vector<int> iv;

 MinA::Communicator<MinA::MPIContext> cx(64);

 if (cx) {
  MinA::Multi<MinA::F<MinA::Simplex<McCormick>>> mc;
  mc.setMetaParameters({ 4, 4, 4, 1 });
  mc.setFileName(".MultiTest");

  auto r = mc.run();
  if (cx == 0) {
   r.print();
  }
 }

 MPI::Finalize();
 return (0);
}
