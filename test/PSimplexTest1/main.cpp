#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"

//#include "MinA/algorithm/SimplexParallel.h"
//#include "MinA/common/Minimizer.h"
//#include "MinA/common/Result.h"
//#include "TestFunction.cpp"
#include "MinA/algorithm/Multi.h"
#include "MinA/common/TestFunctions.hpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
int main(int argc, char** argv)
{

 {

  MinA::Communicator<MinA::MPIContext> cx(3);
  if (cx) {
   {
    if (cx == 0) {
     MinA::Simplex<Schwefel7<5>> mc;
     mc.mpi_procs = 1;
     mc.setMaxIterations(10000);
     mc.setFileName(".PSimplexTest1.simplex.schwefel7.tau0.5");
     auto r = mc.run();
     if (cx == 0) {
      r.print();
     }
    }
    if (cx == 0) {
     MinA::Simplex<Schwefel7<5>> mc;
     mc.setTau(0.8);
     mc.mpi_procs = 1;
     mc.setMaxIterations(10000);
     mc.setFileName(".PSimplexTest1.simplex.schwefel7.tau0.8");
     auto r = mc.run();
     if (cx == 0) {
      r.print();
     }
    }
    if (cx == 0) {
     MinA::Simplex<Schwefel7<5>> mc;
     mc.setTau(0.015);
     mc.mpi_procs = 1;
     mc.setMaxIterations(10000);
     mc.setFileName(".PSimplexTest1.simplex.schwefel7.tau0.015");
     auto r = mc.run();
     if (cx == 0) {
      r.print();
     }
    }
    MinA::ParallelSimplex<Schwefel7<5>> mc;
    mc.mpi_procs = 2;
    mc.setMaxIterations(10000);
    mc.setFileName(".PSimplexTest1.psimplex.schwefel7");
    auto r = mc.run();
    if (cx == 0) {
     r.print();
    }
   }
  }
 }
 MPI::Finalize();
 return (0);
}
