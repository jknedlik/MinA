#include "MinA/algorithm/GSLSimplex.h"
#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

int main(int argc, char** argv)
{

 MinA::Communicator<MinA::MPIContext> cx(2);
 {
  MinA::Communicator<MinA::MPIContext> mx(1);
  if (mx) {
   ScopedTimer a;
   MinA::Simplex<McCormick> mc;
   mc.setMaxIterations(100);
   mc.setFileName(".Simplex.mccormick");
   std::cout << "MinA::Simplex" << std::endl;
   auto r = mc.run();
   if (mx == 0) {
    r.print();
   }
  }

  if (mx) {
   ScopedTimer a;
   MinA::ParallelSimplex<McCormick> mc;
   mc.mpi_procs = 1;
   mc.setMaxIterations(100);
   mc.setFileName(".PSimplex1.mccormick");
   std::cout << "MinA::PSimplex_1" << std::endl;

   auto r = mc.run();
   if (mx == 0) {
    r.print();
   }
  }
  if (mx) {
   ScopedTimer a;
   MinA::GSLSimplex<McCormick> gsl;
   std::cout << "MinA::GSLSimplex" << std::endl;
   auto r = gsl.run();
   if (mx == 0) {
    r.print();
   }
  }
 }
 if (cx) {
  ScopedTimer a;
  MinA::ParallelSimplex<McCormick> mc;
  mc.mpi_procs = 2;
  mc.setMaxIterations(100);
  mc.setFileName(".PSimplex2.mccormick");

  auto r = mc.run();
  if (cx == 0) {
   std::cout << "MinA::PSimplex_2" << std::endl;
   r.print();
  }
 }

 std::vector<int> iv;

 MPI::Finalize();
 return (0);
}
