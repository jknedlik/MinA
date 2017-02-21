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

    {
        MinA::Communicator<MinA::MPIContext> cx(8);
        if (cx) {
            MinA::Multi<MinA::F<MinA::ParallelSimplex<McCormick>>> mc;
            std::vector<size_t> v{ 1, 1, 2, 2 };
            mc.setMetaParameters(
              vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
            mc.filename = ".multi";
            mc.f.alg.filename = ".mulit.psimplex.save.mccorick";
            mc.f.alg.mpi_procs = 2;
            mc.f.mpi_procs = 2;
            mc.mpi_procs = 8;
            mc.f.alg.setMaxIterations(100);
            auto r = mc.run();
            if (cx == 0)
                r.print();

            {
                MinA::Communicator<MinA::MPIContext> cx(2);

                if (cx) {
                    MinA::ParallelSimplex<McCormick> mc;
                    mc.mpi_procs = 2;
                    mc.setMaxIterations(100);

                    mc.filename = ".psimplex.save.mccormick";

                    auto r = mc.run();
                    if (cx == 0) {
                        r.print();
                    }
                }

                if (cx) {
                    MinA::ParallelSimplex<Himmelblau> mc;
                    mc.mpi_procs = 2;
                    mc.setMaxIterations(100);

                    mc.filename = ".psimplex.save.himmelblau";

                    auto r = mc.run();
                    if (cx == 0) {
                        r.print();
                    }
                }
            }
        }

        std::vector<int> iv;
    }
    MPI::Finalize();
    return (0);
}
