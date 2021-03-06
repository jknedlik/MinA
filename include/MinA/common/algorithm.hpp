#ifndef MINA_ALGORITHM
#define MINA_ALGORITHM

#include "MinA/common/tupleserialization.hpp"
#include "MinA/common/utility.hpp"
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <vector>

namespace MinA {
template <typename AlgoInfo, typename MetaPara, typename Function,
          typename ResultType>
class Algorithm {
 protected:
 bool restored;
 bool checkBounds;

 public:
 size_t mpi_procs;

 protected:
 Algorithm()
   : restored(false), mpi_procs(1), checkBounds(true), filename(".algo.save"){};
 virtual void save() const
 {
  std::ofstream out(filename);
  boost::archive::text_oarchive oa(out);
  oa << mMetaParameters;
  oa << mAlgorithmInformations;
 };
 virtual void restore()
 {
  std::ifstream in(filename);
  if (in.good()) {
   boost::archive::text_iarchive ia(in);
   ia >> mMetaParameters;
   ia >> mAlgorithmInformations;
   restored = true;
  }
 };
 virtual void checkBoundaryCondition(typename Function::parametertype& v)
 {
  if (checkBounds) {
   for_each_tuple(v, f.bounds, [](auto& value, auto& boundary) {

    value = std::max(value, boundary.left);
    value = std::min(value, boundary.right);
   });
  }
 }

 public:
 std::string filename;
 AlgoInfo mAlgorithmInformations;
 MetaPara mMetaParameters;
 typename Boundarytuple<MetaPara>::type mMetaBoundaries;

 public:
 Function f;

 auto getAlgorithmInformations() { return mAlgorithmInformations; }
 auto getMetaParameters() { return mMetaParameters; }
 auto getFileName() { return filename; }
 auto getMetaBoundaries() { return mMetaBoundaries; }
 void setAlgorithmInformations(AlgoInfo ai) { mAlgorithmInformations = ai; }
 void setMetaParameters(MetaPara mi) { mMetaParameters = mi; }
 void setFileName(std::string name) { filename = name; }
 void setMetaBoundaries(typename Boundarytuple<MetaPara>::type mb)
 {
  mMetaBoundaries = mb;
 }
 void setCheckBounds(bool toc) { checkBounds = toc; }
 // virtual ResultType run() = 0;
 // virtual auto run() = 0;
 virtual void reset() = 0;
};
template <typename AlgoInfo, typename MetaPara, typename Function>
class Algorithm<AlgoInfo, MetaPara, Function, void>
  : public Algorithm<AlgoInfo, MetaPara, Function,
                     Result<typename Function::parametertype, std::tuple<>>> {
};

} // namespace MinA
#endif
