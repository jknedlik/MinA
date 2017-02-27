#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/SimplexT.h"
#include "MinA/algorithm/Spread.h"
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

    MinA::Communicator<MinA::MPIContext> cx(245);

    if (cx) {
        MinA::Spread<MinA::Simplex<Michalewicz<5>>> mc;
        mc.mpi_procs = 244;
        std::vector<size_t> v{ 3, 3, 3, 3, 3 };
        auto b = vectorToTuple<std::tuple_size<decltype(mc.getMetaParameters())>::value>(v);
        mc.setMetaParameters(b);
        mc.mpi_procs = 243; // 3^5

        auto r = mc.run();
        if (cx == 0) {
            r.print();
        }
    }

    MPI::Finalize();
    return (0);
}
