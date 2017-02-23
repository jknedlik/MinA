#ifndef MINA_TEST
#define MINA_TEST
#include "MinA/common/commonT.hpp"
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
#include <utility>
#include <vector>

namespace MinA {

template <typename... Ts>
struct TupleVector;

template <typename T, typename... Ts>
struct TupleVector<T, Ts...> {
    std::tuple<std::vector<T>> t;
    typename TupleVector<Ts...>::type tf;
    std::tuple<T> nt;
    typename TupleVector<Ts...>::normaltype ntf;
    // decltype NEVER evaluates functions, it just checks for the theoretical
    // return values type, so its fast and fine
    using type = decltype(std::tuple_cat(t, tf));
    using normaltype = decltype(std::tuple_cat(nt, ntf));
};
// base case
template <typename T>
struct TupleVector<T> {
    using type = std::tuple<std::vector<T>>;
    using normaltype = std::tuple<T>;
};
// specialization for tuples so that input <tuple<int,double>> will be converted
// to
// tuple<vector<int>,vector<double>> instead of being nested like
// <tuple<vector<tuple<int,double>>>>
template <typename... Ts>
struct TupleVector<std::tuple<Ts...>> {
    using type = typename TupleVector<Ts...>::type;
    using normaltype = std::tuple<Ts...>;
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

template <typename T, typename... Ts>
struct Boundarytuple<T, Ts...> {
    std::tuple<Bound<T>> t;
    typename Boundarytuple<Ts...>::type ts;
    using type = decltype(std::tuple_cat(t, ts));
};
template <typename T>
struct Boundarytuple<T> {
    using type = std::tuple<Bound<T>>;
};
template <typename... Ts>
struct Boundarytuple<std::tuple<Ts...>> {
    using type = typename Boundarytuple<Ts...>::type;
};

template <typename... Ts>
class Function {
  private:
  public:
    Function() : mpi_procs(1){};
    typename Boundarytuple<Ts...>::type bounds;
    size_t mpi_procs;
    std::string fn;
    using parametertype = std::tuple<Ts...>;
    std::tuple<Ts...> startvalues;

    auto getDim() { return sizeof...(Ts); }
    virtual double evaluate(std::tuple<Ts...> tup) = 0;
};

template <typename... Ts>
class Function<std::tuple<Ts...>> : public Function<Ts...> {
};

template <typename... T>
struct Result;
template <typename... T>
struct Result {

    double value;
    std::tuple<T...> parameters;
    using type = std::tuple<T...>;
    void print()
    {
        std::cout << "Result value: " << value << "\nResult parameter: \n{";
        for_each_tuple(parameters, [](auto x) { std::cout << x << ", "; });
        std::cout << "}" << std::endl;
    }
};
template <typename... T>
struct Result<std::tuple<T...>> : Result<T...> {
};
} // MinA

template <typename T, size_t N>
struct TArray : public TArray<T, N - 1> {
    std::tuple<T> t;
    typename TArray<T, N - 1>::type g;
    using type = decltype(std::tuple_cat(t, g));
};

template <typename T>
struct TArray<T, 1> {
    using type = std::tuple<T>;
};

template <size_t num, typename T>
typename TArray<T, num>::type vectorToTuple(std::vector<T>& v)
{
    typename TArray<T, num>::type tup;
    assert(v.size() == num);
    for_each_tuple_i(tup, [v](size_t index, auto& TE) { TE = v.at(index); });
    return tup;
}

#endif
