#ifndef MINA_TEST
#define MINA_TEST
#include "MinA/common/commonT.hpp"
#include <assert.h>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/array.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace MinA {
template <typename... Ts>
struct TupleVector;

template <typename... Ts>
struct TupleVector {
 using type = std::tuple<std::vector<Ts>...>;
 using normaltype = std::tuple<Ts...>;
};
template <typename... Ts>
struct TupleVector<std::tuple<Ts...>> : TupleVector<Ts...> {
};

template <size_t a, size_t b>
struct less {
 using type = typename std::enable_if<(a < b)>::type;
};
template <typename Tuple, typename TVector, typename F, size_t index>
void constructTuple(Tuple tup, TVector vec, F&& f, std::true_type::type)
{
 f(tup);
}
template <typename Tuple, typename TVector, typename F, size_t index>
void constructTuple(Tuple tup, TVector vec, F&& f, std::false_type::type)
{

 static_assert((std::tuple_size<TVector>::value > index));
 assert(!std::get<index>(vec).empty());
 for (auto i : std::get<index>(vec)) {
  auto icpy = std::make_tuple(i);
  auto cpy = tup;

  constructTuple<decltype(std::tuple_cat(cpy, icpy)), TVector, F, index + 1>(
    std::tuple_cat(cpy, icpy), vec, std::forward<F>(f),
    typename std::integral_constant<bool, ((std::tuple_size<TVector>::value) <=
                                           (index + 1))>::type());
 }
}

template <typename TVector, typename F, size_t index>
void constructTuple(TVector vec, F&& f)
{
 for (auto i : std::get<index>(vec)) {
  auto t = std::make_tuple(i);
  constructTuple<decltype(t), TVector, F, index + 1>(
    t, vec, std::forward<F>(f),
    typename std::integral_constant<bool, ((std::tuple_size<TVector>::value) <=
                                           (index + 1))>::type());
 }
}

template <typename TVector, typename F>
void constructTuple(TVector vec, F&& f)
{
 constructTuple<TVector, F, 0>(vec, std::forward<F>(f));
}

template <typename T>
struct Bound {
 public:
 T left;
 T right;

 private:
 void setleft(T& l) { left = l; }
 void setright(T& r) { right = r; }
};
template <typename... Ts>
struct Boundarytuple;
template <typename... Ts>
struct Boundarytuple {
 using type = std::tuple<Bound<Ts>...>;
};
template <typename... Ts>
struct Boundarytuple<std::tuple<Ts...>> {
 using type = typename Boundarytuple<Ts...>::type;
};

template <typename ParameterType, typename ResultInfo>
struct Result {
 using ptype = ParameterType;
 using rinfo = ResultInfo;
 double value;
 ptype parameters;
 rinfo informations;
 Result() : value(0){};
 Result(double fx) : value(fx) {}
 operator double()
 {
  return value;
 } // (double) cast important for backwards compatability
 void print()
 {
  std::cout << "Result value: " << value << "\nResult parameter: \n{";
  for_each_tuple_i(parameters, [](size_t index, auto x) {
   std::cout << ((index > 0) ? "," : "") << x;
  });
  std::cout << "}\n";
 }
};
template <typename ParameterType>
struct Result<ParameterType, void> : Result<ParameterType, std::tuple<>> {
};

template <typename ps>
class Function {
 public:
 using Basetype = Function<ps>;
 using parametertype = ps;
 using Parameters = ps;
 using boundarytype = typename Boundarytuple<ps>::type;
 using result = Result<parametertype, void>;
 Function() : mpi_procs(1), fn(".Function"){};
 Function(parametertype start, boundarytype bs)
   : startvalues(start), bounds(bs), mpi_procs(1), fn(".Function"){};

 public:
 boundarytype bounds;
 parametertype startvalues;
 std::string fn;
 size_t mpi_procs;
};

} // namespace MinA

template <template <size_t... Is> typename T, size_t... Is>
typename T<Is...>::type indexer(std::index_sequence<Is...> a)
{
 return typename T<Is...>::type();
}
template <template <typename U, size_t... Is> typename T, typename U,
          size_t... Is>
typename T<U, Is...>::type indexer(std::index_sequence<Is...> a)
{
 return typename T<U, Is...>::type();
}
template <typename T, size_t>
struct WRAP {
 using type = T;
};
template <typename T, size_t... Is>
struct ITArray {
 using type = std::tuple<typename WRAP<T, Is>::type...>;
};
template <typename T, size_t N>
struct TArray {
 using type = decltype(indexer<ITArray, T>(std::make_index_sequence<N>()));
};
using namespace MinA;
template <size_t N>
using TA = typename TArray<double, N>::type;
template <size_t N>
TA<N> TranslateVal(TA<N> params, typename Boundarytuple<TA<N>>::type bounds,
                   bool retransform)
{
 TA<N> newparams;
 newparams = params;
 return newparams;
 // return params;
 if (!retransform) {
  for_each_tuple(params, newparams, bounds,
                 [](auto param, auto& newparam, auto& bound) {
                  double a = bound.left;
                  double b = bound.right;
                  newparam = (a + b - 2.0 * param) / (a - b);
                  newparam = newparam / (1.0 - std::fabs(newparam));
                 });
 }
 else {
  for_each_tuple(params, newparams, bounds,
                 [](auto param, auto& newparam, auto& bound) {
                  double a = bound.left;
                  double b = bound.right;
                  if (param > 0.9 || param < -0.9)
                   newparam = param / (1.0 + std::fabs(param));
                  newparam = a + (b - a) * (1 + newparam) / 2.0;
                 });
 }
 return newparams;
}

template <size_t num, typename T>
typename TArray<T, num>::type vectorToTuple(std::vector<T>& v)
{
 typename TArray<T, num>::type tup;
 assert(v.size() == num);
 for_each_tuple_i(tup, [v](size_t index, auto& TE) { TE = v.at(index); });
 return tup;
}

#endif
