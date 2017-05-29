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
 MPI::Init();
 // MinA::Communicator<MinA::MPIContext> cx(200);

 std::tuple<int, int, double, float> tup;
 {
  ScopedTimer a;
  // auto type = createMPIType(tup);
 }

 {
  ScopedTimer a;
  auto trype = getType(tup);
 }
 {
  ScopedTimer a;
  auto trype = getType(tup);
 }
 MPI::Finalize();
 return (0);
}
