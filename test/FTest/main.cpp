#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

int main(int argc, char** argv)
{
 MinA::Simplex<MinA::F<MinA::Simplex<McCormick>>> ss;
 ss.setFileName(".FTest");
 ss.f.fn = ".Ftest.xx";
 ss.f.alg.setMaxIterations(100);
 ss.setMaxIterations(10);
 auto r = ss.run();
 r.print();

 return (0);
}
