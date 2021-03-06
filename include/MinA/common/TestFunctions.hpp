#include "MinA/common/algorithm.hpp"
#include "MinA/common/utility.hpp"
// The global minimum value has been approximated by f(x) =
//−4.687 for n = 5 and by f(x) = −9.66 for n = 10.
template <size_t N>
class Michalewicz : public MinA::Function<typename TArray<double, N>::type> {
 public:
 using result = Result<typename TArray<double, N>::type, void>;
 Michalewicz() : MinA::Function<typename TArray<double, N>::type>::Function()
 {
  this->fn = "MichalewiczN=" + std::to_string(N);

  for_each_tuple(this->startvalues, [](auto& s) { s = 1; });
  for_each_tuple(this->bounds, [](auto& b) {
   b.left = 0;
   b.right = 4 * atan(1);
  });
 }
 result operator()(typename TArray<double, N>::type params)
 {
  result a;
  int i = 1;
  for_each_tuple(params, [&i, &a](auto p) {
   a.value += sin(p) * pow((sin((i * pow(p, 2)) / (4 * atan(1)))), 2 * N);

   i++;
  });
  a.value *= -1;
  return a;
 }
};
template <size_t N>
class Schwefel7 : public MinA::Function<typename TArray<double, N>::type> {
 public:
 using result = Result<typename TArray<double, N>::type, void>;
 Schwefel7() : MinA::Function<typename TArray<double, N>::type>::Function()
 {
  this->fn = "Schwefel7N=" + std::to_string(N);
  for_each_tuple(this->startvalues, [](auto& s) { s = 0; });
  for_each_tuple(this->bounds, [](auto& b) {
   b.left = -500;
   b.right = 500;
  });
 }
 result operator()(typename TArray<double, N>::type params)
 {
  double sum_xs = 0;
  for_each_tuple(params,
                 [&sum_xs](auto p) { sum_xs += p * sin(sqrt(abs(p))); });
  result a;

  a.value = 418.9829 * N - sum_xs;
  return a;
 }
};
// https://www.sfu.ca/~ssurjano/camel6.html
class Camel6 : public MinA::Function<std::tuple<double, double>> {
 public:
 using result = Result<std::tuple<double, double>, void>;
 Camel6() : Basetype()
 {
  this->fn = "Camel6";
  startvalues = decltype(startvalues){ -2.5, -1. };
  bounds = decltype(bounds)({ { -3, 3 }, { -2, 2 } });
 };
 Function::result operator()(std::tuple<double, double> para)
 {
  double x1 = std::get<0>(para);
  double x2 = std::get<1>(para);

  result a;
  a.value = ((4 - 2.1 * pow(x1, 2) + pow(x1, 4) / 3) * pow(x1, 2) + x1 * x2 +
             (-4 + 4 * pow(x2, 2)) * pow(x2, 2));
  return a;
 }
};

class Himmelblau : public MinA::Function<std::tuple<double, double>> {
 public:
 Himmelblau() : Function()
 {
  this->fn = "Himmelblau";
  startvalues = decltype(startvalues){ 3., 4. };
  bounds = decltype(bounds)({ { -10, 10 }, { -10, 10 } });
 };
 typename Function::result operator()(std::tuple<double, double> para)
 {
  double params[2];
  params[0] = std::get<0>(para);
  params[1] = std::get<1>(para);
  result a;
  a.value = std::pow((std::pow(params[0], 2) + params[1] - 11), 2) +
            std::pow(params[0] + (std::pow(params[1], 2) - 7), 2);
  return a;
 }
};

// class McCormick : public MinA::Function<double, double> {
// MCCormick minimum is -1.911 at x=-0.54719,-1.54719
class McCormick : public MinA::Function<std::tuple<double, double>> {
 public:
 McCormick() : Function()
 {
  this->fn = "McCormick";
  this->startvalues = decltype(startvalues)({ 1., 0. });
  bounds = decltype(bounds)({ { -1.5, 4. }, { -3., 4. } });
 };
 result operator()(std::tuple<double, double> params)
 {
  auto a = std::get<0>(params);
  auto b = std::get<1>(params);
  result res;
  res.value = std::sin(a + b) + std::pow(a - b, 2) - 1.5 * a + 2.5 * b + 1;
  return res;
 }
};

template <size_t N>
class MLutz : public MinA::Function<typename TArray<double, N>::type> {
 public:
 using result = Result<typename TArray<double, N>::type, void>;
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
 result operator()(typename TArray<double, N>::type params)
 {
  double sum_xs = 0;
  for_each_tuple(params, [&sum_xs](auto p) { sum_xs += pow(p, 2); });
  result a;
  a.value =
    mAlpha * pow(sin(mBeta * sum_xs), 2) + sum_xs * mGamma * exp(mEta * sum_xs);
  return a;
 }
};
std::tuple<McCormick, Himmelblau, Michalewicz<5>, Schwefel7<5>, Camel6> TFuncs;
