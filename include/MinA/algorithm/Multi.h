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
 using RType =
   decltype(std::declval<Function>()(typename Function::parametertype()));

 std::vector<RType> vec;

 public:
 Multi() : MinA::Algorithm<int, Algoinfo<Function>, Function, void>::Algorithm()
 {
  for_each_tuple(this->mMetaParameters, [this](auto& p) { p = 1; });
 }

 void reset(){};
 void setVec(std::vector<typename Function::parametertype> tovec)
 {
  vec.clear();
  for (auto v : tovec) {
   RType tmp;
   tmp.parameters = v;
   vec.emplace_back(tmp);
  }
 };
 void initializeVec()
 {
  if (vec.empty()) {
   typename TupleVector<typename Function::parametertype>::type tv;
   for_each_tuple(tv, this->f.bounds, this->mMetaParameters,
                  [](auto& ti, auto b, auto mp) {
                   auto step = (b.right - b.left) / (double)(mp + 1);
                   for (int i = 1; i <= mp; i++) {
                    ti.push_back(b.left + i * step);
                   }
                  });
   constructTuple(tv, [this](auto x) {
    RType tmp;
    tmp.parameters = x;
    vec.emplace_back(tmp);
   });
  }
 };
 RType run()
 {
  RType r;
  initializeVec();
  {
   Communicator<MPIContext> all(vec.size() * this->f.mpi_procs);
   if (all) {
    // divide context in same sized pieces by "dividing" the all-context
    Communicator<MPIContext> fEvaluator = all.divide(this->f.mpi_procs);
    // set a nice name for all the files
    this->f.fn = this->filename + "multi." +
                 std::to_string(all.getIdent() / this->f.mpi_procs) +
                 this->f.fn;
    r = this->f(vec[all.getIdent() / this->f.mpi_procs].parameters);
    if (all == 0) { // means i am rank 0
     vec[0] = r;

     for (int i = 1; i < vec.size(); i++) {
      vec[i] = (all.receive<decltype(r)>(i * this->f.mpi_procs));
     }
     sort(vec.begin(), vec.end(),
          [](auto& a, auto& b) -> bool { return a.value < b.value; });
     r = *vec.begin();
     for (auto a : vec) {
     }
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
