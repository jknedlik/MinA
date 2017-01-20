#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/algorithm.hpp"

//#include "MinA/algorithm/SimplexParallel.h"
//#include "MinA/common/Minimizer.h"
//#include "MinA/common/Result.h"
//#include "TestFunction.cpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

template <typename Function>
class TestA : public MinA::Algorithm<int, std::tuple<double, double>, Function> {
  public:
    TestA() { this->mMetaBoundaries = { { 0.0, 2.0 }, { 0.0, 1.0 } }; }
    MinA::Result<typename Function::parametertype> run()
    {
        MinA::Result<typename Function::parametertype> r;
        r.parameters = this->f.startvalues;
        r.value = this->f.evaluate(this->f.startvalues);
        return r;
    };
    void reset(){};
};

// Himmelblau minimia are at

//  f ( 3.0 , 2.0 ) = 0.0
//    f ( − 2.805118 , 3.131312 ) = 0.0
//    f(-3.779310,-3.283186)=0.0
//  f ( 3.584428 , − 1.848126 ) =00

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
        return std::pow((std::pow(params[1], 2) + params[1] - 11), 2) +
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
    void reset()
    {
        startvalues = decltype(startvalues)({ 0., 0. });
        bounds = decltype(bounds)({ { -1.5, 4. }, { -3., 4. } });
    }
    double evaluate(std::tuple<double, double> params)
    {
        auto a = std::get<0>(params);
        auto b = std::get<1>(params);
        double fz = std::sin(a + b) + std::pow(a - b, 2) - 1.5 * a + 2.5 * b + 1;
        return fz;
    }
};
template <typename T, size_t N>
class Gaussian : public MinA::Function<typename TArray<T, N>::type> {
  public:
    Gaussian() : MinA::Function<typename TArray<T, N>::type>::Function()
    {
        for_each(this->startvalues, [](auto& s) { s = 5; });
        for_each(this->bounds, [](auto& i) {
            i.left = -10;
            i.right = 10;
        });
    }
    double evaluate(typename TArray<T, N>::type params)
    {
        double sum_xsin = 0;
        for_each(params, [&sum_xsin](auto para) { sum_xsin += std::pow(para + 1., 2); });
        return -exp(-sum_xsin);
    }
};

int main(int argc, char** argv)
{

    TestA<McCormick> t;
    MinA::Simplex<McCormick> s;
    s.setMaxIterations(1000);
    auto r = s.run();
    r.print();

    return (0);
}
