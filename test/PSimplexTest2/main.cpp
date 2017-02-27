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

        MinA::Communicator<MinA::MPIContext> cx(5);
        if (cx) {

            MinA::ParallelSimplex<Michalewicz<5>> mc;
            mc.mpi_procs = 4;
            mc.setMaxIterations(100);
            mc.setFileName(".PSimplexTest2.psimplex.michael");
            auto r = mc.run();
            if (cx == 0) {
                r.print();
            }
        }
        else {
        }
    }
    MPI::Finalize();
    return (0);
}
