#include "MinA/algorithm/GSLSimplex.h"
#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/algorithm/Spread.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;
template <typename Func>
void alltestMultiPS(Func f)
{
 {
  MinA::Communicator<MinA::MPIContext> cx(8);
  if (cx) {
   MinA::Multi<MinA::F<MinA::ParallelSimplex<Func>>> mc;
   std::vector<size_t> v{ 1, 1, 2, 2 };
   mc.setMetaParameters(
     vectorToTuple<std::tuple_size<decltype(mc.getMetaParameters())>::value>(
       v));
   mc.setFileName(".Multi.psimplex." + mc.f.alg.f.fn);
   mc.f.alg.setFileName(".Multi.single" + std::to_string(cx.getIdent()) +
                        ".psimplex" + mc.f.alg.f.fn);
   mc.f.alg.mpi_procs = 2;
   mc.f.mpi_procs = 2;
   mc.mpi_procs = 8;
   mc.f.alg.setMaxIterations(100);
   auto r = mc.run();
   if (cx == 0) {
    std::cout << "Multi<PSimplex> testing:" << f.fn << std::endl;
    r.print();
   }
  }
 }
}
template <typename Func>
void alltestMultiS(Func f)
{
 {
  MinA::Communicator<MinA::MPIContext> cx(4);
  if (cx) {
   MinA::Multi<MinA::F<MinA::Simplex<Func>>> mc;
   std::vector<size_t> v{ 1, 1, 2, 2 };
   mc.setMetaParameters(
     vectorToTuple<std::tuple_size<decltype(mc.getMetaParameters())>::value>(
       v));
   mc.setFileName(".Multi.simplex." + mc.f.alg.f.fn);
   mc.f.alg.setFileName(".Multi.single" + std::to_string(cx.getIdent()) +
                        ".simplex" + mc.f.alg.f.fn);
   mc.f.alg.mpi_procs = 1;
   mc.f.mpi_procs = 1;
   mc.mpi_procs = 4;
   mc.f.alg.setMaxIterations(100);
   auto r = mc.run();
   if (cx == 0) {
    std::cout << "Multi<Simplex> testing:" << f.fn << std::endl;
    r.print();
   }
  }
 }
}
template <typename Func>
void alltestPS(Func f)
{
 {
  MinA::Communicator<MinA::MPIContext> cx(
    std::tuple_size<typename Func::parametertype>::value - 1);
  if (cx) {
   MinA::ParallelSimplex<Func> ps;
   ps.setFileName(".Psimplex." + ps.f.fn);
   ps.mpi_procs = std::tuple_size<typename Func::parametertype>::value - 1;
   ps.setMaxIterations(100);
   auto r = ps.run();
   if (cx == 0) {
    std::cout << "PSimplex testing:" << f.fn << std::endl;
    r.print();
   }
  }
 }
}
template <typename Func>
void alltestS(Func f)
{
 {
  MinA::Communicator<MinA::MPIContext> cx(1);
  if (cx) {
   MinA::Simplex<Func> s;
   s.setFileName(".Simplex." + s.f.fn);
   s.mpi_procs = 1;
   s.setMaxIterations(100);
   auto r = s.run();
   if (cx == 0) {
    std::cout << "Simplex testing:" << f.fn << std::endl;
    r.print();
   }
  }
 }
}
template <typename Func>
void alltestSpread(Func f)
{
 size_t s = 1;
 for (int i = 0; i < std::tuple_size<typename Func::parametertype>::value;
      i++) {
  s *= (i < 5) ? 2 : 1;
 }
 MinA::Communicator<MinA::MPIContext> cx(s + 1);

 if (cx) {
  MinA::Spread<MinA::Simplex<Func>> mc;
  mc.mpi_procs = s;
  std::vector<size_t> v;
  v.resize(std::tuple_size<decltype(mc.getMetaParameters())>::value);
  for (int i = 0; i < v.size(); i++) {
   v[i] = (i < 5) ? 2 : 1;
  }
  mc.setFileName(".spread." + mc.f.fn);
  auto b =
    vectorToTuple<std::tuple_size<decltype(mc.getMetaParameters())>::value>(v);
  mc.setMetaParameters(b);

  auto r = mc.run();
  if (cx == 0) {
   std::cout << "Spread<Simplex> testing:" << f.fn << std::endl;
   r.print();
  }
 }
}
template <typename Func>
void alltestGSLSimplex(Func f)
{
 MinA::Communicator<MinA::MPIContext> cx(1);
 if (cx) {
  MinA::GSLSimplex<Func> gsl;
  auto r = gsl.run();
  std::cout << "GSLSimplex testing:" << f.fn << std::endl;
  r.print();
 }
}

int main(int argc, char** argv)
{
 for_each_tuple(TFuncs, [](auto func) {
  alltestS(func);
  alltestPS(func);
  alltestMultiPS(func);
  alltestMultiS(func);
  alltestSpread(func);
  alltestGSLSimplex(func);
 });
 MPI::Finalize();
 return (0);
}
