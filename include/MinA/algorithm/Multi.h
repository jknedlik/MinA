#ifndef MULTIT_H
#define MULTIT_H
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include "MinA/common/utility.hpp"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#define MULTI_NUMBER = 0;
namespace MinA {
template <typename Function>
using Algoinfo = typename TArray<
  size_t, std::tuple_size<typename Function::parametertype>::value>::type;

template <typename Function>

class Multi : public MinA::Algorithm<int, Algoinfo<Function>, Function, void> {
 public:
 Multi() : MinA::Algorithm<int, Algoinfo<Function>, Function, void>::Algorithm()
 {
  for_each_tuple(this->mMetaParameters, [](auto& p) { p = 1; });
 }
 void reset(){};
 typename Function::result run()
 {

  decltype(this->f(typename Function::parametertype())) r;
  typename TupleVector<typename Function::parametertype>::type tv;
  std::vector<std::pair<double, typename Function::parametertype>> vec;
  for_each_tuple(tv, this->f.bounds, this->mMetaParameters,
                 [](auto& ti, auto b, auto mp) {
                  auto step = (b.right - b.left) / (double)(mp + 1);
                  for (int i = 1; i <= mp; i++) {
                   ti.push_back(b.left + i * step);
                  }
                 });
  constructTuple(tv, [&vec](auto x) { vec.emplace_back(0.0, x); });
  {
   Communicator<MPIContext> all(vec.size() * this->f.mpi_procs);
   if (all) {
    // divide context in same sized pieces by "dividing" the all-context
    Communicator<MPIContext> fEvaluator = all.divide(this->f.mpi_procs);
    // set a nice name for all the files
    this->f.fn = this->filename + "multi." +
                 std::to_string(all.getIdent() / this->f.mpi_procs) +
                 this->f.fn;

    r = this->f(vec[all.getIdent() / this->f.mpi_procs].second);
    if (all == 0) { // means i am rank 0
     std::vector<decltype(r)> rvec;
     rvec.push_back(r);

     for (int i = 1; i < vec.size(); i++) {
      rvec.push_back(all.receive<decltype(r)>(i * this->f.mpi_procs));
     }
     sort(rvec.begin(), rvec.end(),
          [](auto& a, auto& b) -> bool { return a.value < b.value; });
     r = *rvec.begin();
    }
    else {
     // only rank zero of one evaluator context returns the result back to the
     // head
     // of the multi context  (rank 0 of all)
     if (fEvaluator.getIdent() == 0)
      all.send(0, r);
    }
   }
  }
  return r;
 }
};
} // namespace MinA
#endif
