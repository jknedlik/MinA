#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/SimplexT.h"
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

    MinA::Communicator<MinA::MPIContext> cx(80);

    if (cx) {
        MinA::Multi<MinA::F<MinA::Simplex<McCormick>>> mc;
        std::vector<size_t> v{ 2, 2, 5, 4 };
        mc.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
        mc.filename = ".multi.save";

        auto r = mc.run();
        if (cx == 0) {
            r.print();
        }
    }

    MPI::Finalize();
    return (0);
}
