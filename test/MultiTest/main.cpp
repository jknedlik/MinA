#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"

//#include "MinA/algorithm/SimplexParallel.h"
//#include "MinA/common/Minimizer.h"
//#include "MinA/common/Result.h"
//#include "TestFunction.cpp"
#include "MinA/algorithm/Multi.h"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

class Himmelblau : public MinA::Function<double, double> {
  public:
    Himmelblau() : Function()
    {
        startvalues = decltype(startvalues){ 3., 4. };
        bounds = decltype(bounds)({ { -10, 10 }, { -10, 10 } });
    };
    double evaluate(std::tuple<double, double> para)
    {
        double params[2];
        params[0] = std::get<0>(para);
        params[1] = std::get<1>(para);
        return std::pow((std::pow(params[0], 2) + params[1] - 11), 2) +
               std::pow(params[0] + (std::pow(params[1], 2) - 7), 2);
    }
};

// class McCormick : public MinA::Function<double, double> {
// MCCormick minimum is -1.911 at x=-0.54719,-1.54719
class McCormick : public MinA::Function<double, double> {
  public:
    McCormick() : Function()
    {
        startvalues = decltype(startvalues)({ 0., 0. });
        bounds = decltype(bounds)({ { -1.5, 4. }, { -3., 4. } });
    };
    double evaluate(std::tuple<double, double> params)
    {
        auto a = std::get<0>(params);
        auto b = std::get<1>(params);
        double fz = std::sin(a + b) + std::pow(a - b, 2) - 1.5 * a + 2.5 * b + 1;
        return fz;
    }
};

template <size_t N>
class MLutz : public MinA::Function<typename TArray<double, N>::type> {
  public:
    double mAlpha;
    double mBeta;
    double mGamma;
    double mEta;
    ;
    MLutz()
      : mAlpha(6.),
        mBeta(3.),
        mGamma(1.),
        mEta(1.),
        MinA::Function<typename TArray<double, N>::type>::Function()
    {
        int i = 0;
        for_each_tuple(this->startvalues, [&i](auto& s) {
            s = i * 0.5;
            i++;
        });
        for_each_tuple(this->bounds, [](auto& b) {
            b.left = -5;
            b.right = 5;
        });
    }
    double evaluate(typename TArray<double, N>::type params)
    {
        double sum_xs = 0;
        for_each_tuple(params, [&sum_xs](auto p) { sum_xs += pow(p, 2); });
        std::cout << "sumyx" << sum_xs << std::endl;
        double fz = mAlpha * pow(sin(mBeta * sum_xs), 2) + sum_xs * mGamma * exp(mEta * sum_xs);
        return fz;
    }
};
int main(int argc, char** argv)
{
    std::vector<int> iv;

    MinA::Communicator<MinA::MPIContext> cx(80);

    if (cx) {
        MinA::Multi<MinA::F<MinA::Simplex<McCormick>>> mc;
        std::vector<size_t> v{ 2, 2, 5, 4 };
        mc.setMetaParameters(
          vectorToTuple<std::tuple_size<decltype(mc.mMetaParameters)>::value>(v));
        mc.filename = ".multi.save";
        auto r = mc.run();
        if (cx == 0) {
            r.print();
        }
    }

    MPI::Finalize();
    return (0);
}
