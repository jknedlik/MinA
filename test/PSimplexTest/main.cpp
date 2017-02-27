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

    if (cx) {
        MinA::Simplex<McCormick> mc;
        mc.setMaxIterations(100);
        mc.setFileName(".PSimplex.mccormick");

        auto r = mc.run();
        if (cx == 0) {
            r.print();
        }
    }

    if (cx) {
        MinA::ParallelSimplex<McCormick> mc;
        mc.mpi_procs = 2;
        mc.setMaxIterations(100);
        mc.setFileName(".PSimplex.mccormick");

        auto r = mc.run();
        if (cx == 0) {
            r.print();
        }
    }

    std::vector<int> iv;

    MPI::Finalize();
    return (0);
}
