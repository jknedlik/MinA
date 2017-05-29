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
   for_each_tuple_i(r.informations,
                    [](size_t i, auto& r) { std::cout << r << std::endl; });
  }
 }

 {
  MinA::Communicator<MinA::MPIContext> cy(2);
  if (cy) {
   if (cy == 0) {
    Result<std::tuple<double, double>, int> p;
    p.value = 10;
    p.parameters = { 20, 20 };
    p.informations = 2;
    cy.send(1, p);
   }
   else {
    auto p = cy.receive<Result<std::tuple<double, double>, int>>(0);
    p.print();
    std::cout << "inform:" << p.informations << std::endl;
   }
  }
 }

 MPI::Finalize();
 return (0);
}
