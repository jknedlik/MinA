#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/SimplexT.h"
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
using namespace std;

int main(int argc, char** argv)
{
    std::vector<int> iv;

    MinA::Communicator<MinA::MPIContext> cx(4 * 4 * 4 * 2);

    if (cx) {
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<10>>>> mc10;
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<5>>>> mc5;
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<2>>>> mc2;
        std::vector<size_t> v{ 2, 4, 4, 4 };
        mc5.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc5.mMetaParameters)>::value>(v));
        mc5.filename = ".multi5.save";
        mc2.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc2.mMetaParameters)>::value>(v));
        mc2.filename = ".multi2.save";
        mc10.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc10.mMetaParameters)>::value>(v));
        mc10.filename = ".multi10.save";

        auto r10 = mc10.run();
        auto r5 = mc5.run();
        auto r2 = mc2.run();

        if (cx == 0) {
            r10.print();
            r5.print();
            r2.print();
        }
    }

    MPI::Finalize();
    return (0);
}
