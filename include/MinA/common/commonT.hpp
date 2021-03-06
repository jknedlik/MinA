#ifndef MINA_COMMONT
#define MINA_COMMONT
#include <chrono>
#include <iostream>
#include <tuple>
#include <type_traits>

// This is the type which holds sequences
template <int... Ns>
struct sequence {
};
// First define the template signature
template <int... Ns>
struct seq_gen;
// Recursion case
template <int I, int... Ns>
struct seq_gen<I, Ns...> {
 // Take front most number of sequence,
 // decrement it, and prepend it twice.
 // First I - 1 goes into the counter,
 // Second I - 1 goes into the sequence.
 using type = typename seq_gen<I - 1, I - 1, Ns...>::type;
};
// Recursion abort
template <int... Ns>
struct seq_gen<0, Ns...> {
 using type = sequence<Ns...>;
};

template <int N>
using sequence_t = typename seq_gen<N>::type;

namespace detail {
template <std::size_t Ofst, typename Tuple, std::size_t... I>
constexpr auto slice_impl(Tuple&& t, std::index_sequence<I...>)
{
 return std::forward_as_tuple(std::get<I + Ofst>(std::forward<Tuple>(t))...);
}
} // namespace detail

template <std::size_t I1, std::size_t I2, typename Cont>
constexpr auto tuple_slice(Cont&& t)
{
 static_assert(I2 >= I1, "invalid slice");
 static_assert(std::tuple_size<std::decay_t<Cont>>::value >= I2,
               "slice index out of bounds");

 return detail::slice_impl<I1>(std::forward<Cont>(t),
                               std::make_index_sequence<I2 - I1>{});
}

template <typename, typename>
struct concat_tuple {
};

template <size_t index, typename F, typename Tupletuple, std::size_t... Indices>
void for_eachn_impl_impl_impl(F&& f, Tupletuple&& t,
                              std::index_sequence<Indices...>)
{
 f(std::get<index>(std::get<Indices>(t))...), void();
};
template <typename F, typename Tupletuple, size_t index>
void for_eachn_impl_impl(F&& f, Tupletuple&& t)
{
 constexpr size_t N = std::tuple_size<Tupletuple>::value;
 for_eachn_impl_impl_impl<index>(std::forward<F>(f),
                                 std::forward<Tupletuple>(t),
                                 std::make_index_sequence<N>{});
}

template <typename F, typename Tupletuple, std::size_t... Indices>
void for_eachn_impl(F&& f, Tupletuple&& t, std::index_sequence<Indices...>)
{
 using swallow = int[];
 (void)swallow{ 1, (void(for_eachn_impl_impl<F, Tupletuple, Indices>(
                      std::forward<F>(f), std::forward<Tupletuple>(t))),
                    0)... };
}

template <typename F, typename Tupletuple>
void for_eachn(Tupletuple&& t, F&& f)
{
 constexpr size_t N =
   std::tuple_size<typename std::remove_reference_t<typename std::tuple_element<
     0, std::remove_reference_t<Tupletuple>>::type>>::value;
 for_eachn_impl(std::forward<F>(f), std::forward<Tupletuple>(t),
                std::make_index_sequence<N>{});
}

template <typename... Tuples>
void for_each_tuple(Tuples&&... arg)
{
 auto tup = std::forward_as_tuple(arg...);
 auto slice = tuple_slice<0, sizeof...(Tuples) - 1>(
   std::forward<std::tuple<Tuples...>>(tup));
 auto f =
   std::get<sizeof...(Tuples) - 1>(std::forward<std::tuple<Tuples...>>(tup));
 for_eachn<decltype(f), decltype(slice)>(std::forward<decltype(slice)>(slice),
                                         std::forward<decltype(f)>(f));
};

template <size_t... I>
constexpr auto indexTuple(std::index_sequence<I...> ix)
{
 return std::make_tuple(I...);
}

template <typename... Tuples>
void for_each_tuple_i(Tuples&&... arg)
{
 using firstT =
   typename std::tuple_element<0, typename std::tuple<Tuples...>>::type;
 constexpr size_t tuplesize =
   std::tuple_size<std::remove_reference_t<firstT>>::value;
 constexpr auto x = indexTuple(std::make_index_sequence<tuplesize>());
 for_each_tuple(x, std::forward<Tuples>(arg)...);
}

class ScopedTimer {
 private:
 std::chrono::high_resolution_clock::time_point t1;
 std::chrono::high_resolution_clock::time_point t2;
 bool nowrite;

 public:
 ScopedTimer(bool nowrite = true) : nowrite(nowrite)
 {
  t1 = std::chrono::high_resolution_clock::now();
 }
 ~ScopedTimer()
 {
  t2 = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_span =
    std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
  if (!nowrite)
   std::cout << "ScopedTimer took: " << time_span.count() << "seconds"
             << std::endl;
 }
};

#endif
