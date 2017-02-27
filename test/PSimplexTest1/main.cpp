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
                    MinA::Simplex<Michalewicz<5>> mc;
                    mc.mpi_procs = 1;
                    mc.setMaxIterations(100);
                    mc.setFileName(".PSimplexTest1.michael");
                    auto r = mc.run();
                    if (cx == 0) {
                        r.print();
                    }
                }

                MinA::ParallelSimplex<Michalewicz<5>> mc;
                mc.mpi_procs = 2;
                mc.setMaxIterations(100);
                mc.setFileName(".PSimplexTest1.michael");
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
