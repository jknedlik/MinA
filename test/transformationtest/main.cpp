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
/*
template <size_t N>
using TA = typename TArray<double, N>::type;
template <size_t N>
TA<N> TranslateVal(TA<N> params, typename Boundarytuple<TA<N>>::type bounds,
                   bool retransform)
{
 TA<N> newparams;
 if (!retransform) {
  for_each_tuple(params, newparams, bounds,
                 [](auto& param, auto& newparam, auto& bound) {
                  double a = bound.left;
                  double b = bound.right;
                  newparam = (a + b - 2.0 * param) / (a - b);
                  if (newparam < 0)
                   newparam = newparam / (1.0 - newparam);
                  else
                   newparam = newparam / (1.0 + newparam);

                 });
 }
 else {
  for_each_tuple(params, newparams, bounds,
                 [](auto& param, auto& newparam, auto& bound) {
                  double a = bound.left;
                  double b = bound.right;

                  if (param < 0)
                   newparam = param / (1 + param);
                  else
                   newparam = param / (1 - param);
                  newparam = a + (b - a) * (1 + newparam) / 2.0;

                 });
 }
 return newparams;
}
*/
int main(int argc, char** argv)
{
 Camel6 f;
 auto v = f.startvalues;
 auto b = f.bounds;
 // typename Boundarytuple<TA<3>>::type b{ { 10, 100 }, { 0, 10 }, { 300, 400 }
 // };
 std::cout << "\n"
           << "Pre Transform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });
 v = TranslateVal<2>(v, b, false);
 std::cout << "\n"
           << "Post Transform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });
 v = TranslateVal<2>(v, b, true);

 std::cout << "\n"
           << "Post ReTransform:" << std::endl;
 for_each_tuple(v, [](auto& p) { std::cout << p << ","; });

 return (0);
}
