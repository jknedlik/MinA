#ifndef MINA_PSIMPLEX
#define MINA_PSIMPLEX
#include "MinA/algorithm/SimplexT.h"
#include "mpi.h"
namespace MinA {
template <typename Function>
using vertex = std::pair<typename Function::parametertype, double>;
template <typename Function>
using simplext =
  std::array<vertex<Function>, std::tuple_size<decltype(Function::startvalues)>::value + 1>;
template <typename Function>
using ai = std::tuple<size_t, size_t, typename Function::parametertype, simplext<Function>>;
template <typename Function>
class ParallelSimplex : public Simplex<Function> {
    ParallelSimplex() : Simplex<Function>(){};
    Result<typename Function::parametertype> run()
    {

        constexpr size_t mDimension = std::tuple_size<typename Function::parametertype>::value;
    }

    vertex<Function> getContractedPoint(vertex<Function>& M, vertex<Function>& Ajp)
    {
        // Return contraction point Ac of initial point Ajp w.r.t. centroid M
        // Ac = beta * (Ajp + M)
        //
        vertex<Function> Ac;

        for_each_tuple(Ac.first, Ajp.first, M.first, [this](auto& ac, auto& ajp, auto& m) {
            ac = std::get<SIMPLEXT_BETA>(this->mMetaParameters) * (ajp + m);
        });

        Ac.second = this->f.evaluate(Ac.first);
        return Ac;
    }
};
}
#endif
