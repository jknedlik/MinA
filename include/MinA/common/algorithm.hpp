#ifndef MINA_ALGORITHM
#define MINA_ALGORITHM

#include "MinA/common/test.hpp"
#include "MinA/common/tupleserialization.hpp"
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
template <typename AlgoInfo, typename MetaPara, typename Function>
class Algorithm {
  protected:
    bool restored;

  public:
    size_t mpi_procs;

  protected:
    Algorithm() : restored(false), mpi_procs(1), filename(".algo.save"){};
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
        for_each_tuple(v, f.bounds, [](auto& value, auto& boundary) {

            value = std::max(value, boundary.left);
            value = std::min(value, boundary.right);
        });
    }

  public:
    std::string filename;
    AlgoInfo mAlgorithmInformations;
    MetaPara mMetaParameters;
    typename Boundarytuple<MetaPara>::type mMetaBoundaries;
    Function f;
    void setAlgorithmInformations(AlgoInfo ai) { mAlgorithmInformations = ai; }
    void setMetaParameters(MetaPara mi) { mMetaParameters = mi; }
    void setFileName(std::string name) { filename = name; }
    virtual Result<typename Function::parametertype> run() = 0;
    virtual void reset() = 0;
};
} // MinA
#endif
