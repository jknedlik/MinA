#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include <iostream>
#include <map>
#include <utility>
using namespace std;
using mTup = TArray<size_t, 4>::type;
size_t mtup_mpi(mTup tup)
{
 size_t res = 1;
 for_each_tuple(tup, [&res](auto& t) { res *= t; });
 return res;
}
template <size_t N>
auto create_mt(size_t V)
{
 typename TArray<size_t, N>::type tup;
 for_each_tuple(tup, [V](auto& t) { t = V; });
 return tup;
}

int main(int argc, char** argv)
{
 auto test = std::make_tuple(2, 2, 2, 1);
 for_each_tuple_i(test, [](size_t index, auto& te) {
  auto tup = create_mt<4>(te);
  MinA::Communicator<MinA::MPIContext> cx(mtup_mpi(tup) * 5);
  if (cx) {
   MinA::Multi<MinA::F<MinA::ParallelSimplex<Michalewicz<10>>>> mc;
   mc.setMetaParameters(tup);
   mc.f.mpi_procs = 5;
   mc.f.alg.mpi_procs = 5;
   mc.setFileName(".MultiTest" + std::to_string(index));
   auto r = mc.run();
   if (cx == 0) {
    r.print();
    auto paras = r.informations;
    std::cout << "parameters:" << std::endl;
    for_each_tuple_i(paras, [](size_t index, auto x) {
     std::cout << "para #" << index << ": " << x << std::endl;
    });
   }
  }
 });
 MPI::Finalize();
 return (0);
}
