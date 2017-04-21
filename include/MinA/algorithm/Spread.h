#ifndef SPREAD_H
#define SPREAD_H
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
template <typename Algo>
using MFUNCTION = decltype(Algo::f);
template <typename Algo>
using paratype = typename MFUNCTION<Algo>::parametertype;
template <typename Algo>
using ftype =
  typename TArray<size_t, std::tuple_size<paratype<Algo>>::value>::type;

template <typename Algo>
class Spread : public MinA::Algorithm<int, ftype<Algo>, MFUNCTION<Algo>> {
 public:
 Spread() : MinA::Algorithm<int, ftype<Algo>, MFUNCTION<Algo>>::Algorithm()
 {
  for_each_tuple(this->mMetaParameters, [](auto& p) { p = 1; });
 }
 void reset(){};
 Result<paratype<Algo>> run()
 {
  typename TupleVector<paratype<Algo>>::type tv;
  std::vector<std::pair<double, paratype<Algo>>> vec;
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
   double r;
   if (all) {
    // divide context in same sized pieces by "dividing" the all-context
    Communicator<MPIContext> fEvaluator = all.divide(this->f.mpi_procs);
    std::vector<Algo> avector;
    avector.resize(vec.size());

    for (int i = 0; i < vec.size(); i++) {
     avector.at(i).f.startvalues = vec.at(i).second;
     avector.at(i).setFileName(this->filename + std::to_string(i) +
                               avector.at(i).getFileName());
    }
    // set a nice name for all the files
    this->f.fn = this->filename + "spread." +
                 std::to_string(all.getIdent() / this->f.mpi_procs) +
                 this->f.fn;

    auto r = avector[all.getIdent() / this->f.mpi_procs].run();

    if (all == 0) { // means i am rank 0
     vec[0].first = r.value;
     for (int i = 1; i < vec.size(); i++) {
      std::tuple<double> tmp =
        all.receive<std::tuple<double>>(i * this->f.mpi_procs);
      vec[i].first = std::get<0>(tmp);
     }
     sort(vec.begin(), vec.end(),
          [](auto& a, auto& b) -> bool { return a.first < b.first; });
    }
    else {
     // only rank zero of one evaluator context returns the result back to the
     // head
     // of the multi context  (rank 0 of all)
     if (fEvaluator.getIdent() == 0)
      all.send(0, std::tuple<double>(r.value));
    }
   }
  }
  MinA::Result<paratype<Algo>> result;
  result.parameters = vec[0].second;
  result.value = vec[0].first;
  return result;
 }
};
}
#endif
