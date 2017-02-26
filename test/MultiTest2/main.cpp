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
        Michalewicz<2> m2;
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<10>>>> mc10;
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<5>>>> mc5;
        MinA::Multi<MinA::F<MinA::Simplex<Michalewicz<2>>>> mc2;
        std::vector<size_t> v{ 2, 4, 4, 4 };
        mc5.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc5.mMetaParameters)>::value>(v));
        mc5.filename = ".MultiTest2.mc5";
        mc2.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc2.mMetaParameters)>::value>(v));
        mc2.filename = ".Multitest2.mc2";
        mc10.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc10.mMetaParameters)>::value>(v));
        mc10.filename = ".Multitest2.mc10";

        auto r10 = mc10.run();
        auto r5 = mc5.run();
        auto r2 = mc2.run();

        if (cx == 0) {
            double eval = m2.evaluate(std::tuple<double, double>(2.20, 1.57));
            std::cout << "M2 bestat 2.20,1.57:" << eval << std::endl;
            r10.print();
            r5.print();
            r2.print();
        }
    }

    MPI::Finalize();
    return (0);
}
