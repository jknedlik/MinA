#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include "mpi.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
using namespace MinA;

int main(int argc, char** argv)
{
 Camel6 f;
 auto v = f.startvalues;
 auto b = f.bounds;
 auto vx = f.startvalues;
 auto b1 = std::get<0>(f.bounds).left;
 auto b2 = std::get<1>(f.bounds).right;
 auto crit = ((b2 - b1) * 0.01) / 4.f;
 std::get<0>(vx) = b1 + crit;
 std::get<1>(vx) = b2 - crit;
 // typename Boundarytuple<TA<3>>::type b{ { 10, 100 }, { 0, 10 }, { 300, 400 }
 // };
 std::cout << "\n"
           << "Pre Transform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });
 for_each_tuple(vx, [](auto& p) { std::cout << p << ","; });
 v = TranslateVal<2>(v, b, false);
 vx = TranslateVal<2>(vx, b, false);
 std::cout << "\n"
           << "Post Transform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });
 for_each_tuple(vx, [](auto& p) { std::cout << p << ","; });
 v = TranslateVal<2>(v, b, true);
 vx = TranslateVal<2>(vx, b, true);
 std::cout << "\n"
           << "Post ReTransform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });
 for_each_tuple(vx, [](auto& p) { std::cout << p << ","; });

 return (0);
}
