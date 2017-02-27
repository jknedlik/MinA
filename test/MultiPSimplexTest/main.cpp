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
            MinA::Multi<MinA::F<MinA::ParallelSimplex<Michalewicz<5>>>> mc;
            std::vector<size_t> v{ 1, 1, 2, 2 };
            mc.setMetaParameters(
              vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
            mc.filename = ".MultiPSimplexTest";
            mc.f.alg.filename = ".psimplexmulti4.save.mccormick";
            mc.f.alg.mpi_procs = 2;
            mc.f.mpi_procs = 2;
            mc.mpi_procs = 8;
            mc.f.alg.setMaxIterations(100);
            auto r = mc.run();
            if (cx == 0)
                r.print();
        }
        if (cx) {
            MinA::Multi<MinA::F<MinA::ParallelSimplex<Schwefel7<5>>>> mc;
            std::vector<size_t> v{ 1, 1, 2, 2 };
            mc.setMetaParameters(
              vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
            mc.filename = ".MultiPSimplexTest.schwefel7";
            mc.f.alg.filename = ".psimplexmulti4.save.schwefel7";
            mc.f.alg.mpi_procs = 2;
            mc.f.mpi_procs = 2;
            mc.mpi_procs = 8;
            mc.f.alg.setMaxIterations(100);
            auto r = mc.run();
            if (cx == 0)
                r.print();
        }
        if (cx) {
            MinA::Multi<MinA::F<MinA::ParallelSimplex<Camel6>>> mc;
            std::vector<size_t> v{ 1, 1, 2, 2 };
            mc.setMetaParameters(
              vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
            mc.filename = ".MultiPSimplexTest.schwefel7";
            mc.f.alg.filename = ".psimplexmulti4.save.schwefel7";
            mc.f.alg.mpi_procs = 2;
            mc.f.mpi_procs = 2;
            mc.mpi_procs = 8;
            mc.f.alg.setMaxIterations(100);
            auto r = mc.run();
            if (cx == 0)
                r.print();
        }
    }
    MPI::Finalize();
    return (0);
}
