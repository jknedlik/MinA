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

    MinA::Communicator<MinA::MPIContext> cx(300);

    if (cx) {
        auto t = std::make_tuple(cx.getIdent());
        for (int i = 0; i < 1000; i++) {
            if (i % 300 == cx.getIdent()) {
                // cx.broadcast(t);
                for (int j = 0; j < cx.getSize(); j++)
                    cx.send(j, t);
            }
            else {
                auto y = cx.receive<decltype(t)>(i % 300);
                std::cout << std::get<0>(y) << std::endl;
            }
        }
    }
    MPI::Finalize();
    return (0);
}
