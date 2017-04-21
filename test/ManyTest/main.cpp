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
using namespace std;

template <size_t N>
class NTest : public MinA::Function<typename TArray<double, N>::type> {
  public:
    NTest() : MinA::Function<typename TArray<double, N>::type>::Function() {}
    double evaluate(typename TArray<double, N>::type params)
    {
        std::vector<double> vec;
        for_each_tuple(params, [&vec](auto val) { vec.push_back(val); });

        double x = 0;
        for (auto v : vec) {
            x += v;
        }
        return x;
    }
};
template <size_t N>
struct seqqer : public seqqer<N - 1> {
    using ltype = std::tuple<NTest<N>>;
    using type = decltype(std::tuple_cat(ltype(), typename seqqer<N - 1>::type()));
};
template <>
struct seqqer<1> {
    using type = std::tuple<NTest<1>>;
};
int main(int argc, char** argv)
{
    seqqer<100>::type allf;
    TArray<double, 11>::type t{ 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0 };
    for_each_tuple_i(allf, [t](size_t i, auto x) {
        if (i == sizeof(t))
            std::cout << x.evaluate(t) << std::endl;

    });
    //  MPI::Finalize();
    return (0);
}
