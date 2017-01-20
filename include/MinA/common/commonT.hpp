#ifndef MINA_COMMONT
#define MINA_COMMONT
#include <tuple>
#include <type_traits>

namespace detail {
template <std::size_t Ofst, typename Tuple, std::size_t... I>
constexpr auto slice_impl(Tuple&& t, std::index_sequence<I...>)
{
    return std::forward_as_tuple(std::get<I + Ofst>(std::forward<Tuple>(t))...);
}
}

template <std::size_t I1, std::size_t I2, typename Cont>
constexpr auto tuple_slice(Cont&& t)
{
    static_assert(I2 >= I1, "invalid slice");
    static_assert(std::tuple_size<std::decay_t<Cont>>::value >= I2, "slice index out of bounds");

    return detail::slice_impl<I1>(std::forward<Cont>(t), std::make_index_sequence<I2 - I1>{});
}

template <typename, typename>
struct concat_tuple {
};
/*
template <typename Tuple, typename F, std::size_t... Indices>
void for_each_impl(Tuple&& tuple, F&& f, std::index_sequence<Indices...>)
{
    using swallow = int[];
    (void)swallow{ 1, (f(std::get<Indices>(std::forward<Tuple>(tuple))), void(), int{})... };
}

template <typename Tuple, typename F>
void for_each(Tuple&& tuple, F&& f)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<Tuple>>::value;
    for_each_impl(std::forward<Tuple>(tuple), std::forward<F>(f), std::make_index_sequence<N>{});
}

template <typename TupleA, typename TupleB, typename F, std::size_t... Indices>
void for_each2_impl(TupleA&& tuplea, TupleB&& tupleb, F&& f, std::index_sequence<Indices...>)
{
    using swallow = int[];
    (void)swallow{ 1, (f(std::get<Indices>(std::forward<TupleA>(tuplea)),
                         std::get<Indices>(std::forward<TupleB>(tupleb))),
                       void(), int{})... };
}

template <typename TupleA, typename TupleB, typename TupleC, typename F, std::size_t... Indices>
void for_each3_impl(TupleA&& tuplea, TupleB&& tupleb, TupleC&& tuplec, F&& f,
                    std::index_sequence<Indices...>)
{
    using swallow = int[];
    (void)swallow{ 1, (f(std::get<Indices>(std::forward<TupleA>(tuplea)),
                         std::get<Indices>(std::forward<TupleB>(tupleb)),
                         std::get<Indices>(std::forward<TupleC>(tuplec))),
                       void(), int{})... };
}
template <typename TupleA, typename TupleB, typename TupleC, typename F>
void for_each3(TupleA&& tuplea, TupleB&& tupleb, TupleC&& tuplec, F&& f)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<TupleA>>::value;
    for_each3_impl(std::forward<TupleA>(tuplea), std::forward<TupleB>(tupleb),
                   std::forward<TupleC>(tuplec), std::forward<F>(f), std::make_index_sequence<N>{});
}
template <typename TupleA, typename TupleB, typename F>
void for_each2(TupleA&& tuplea, TupleB&& tupleb, F&& f)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<TupleA>>::value;
    for_each2_impl(std::forward<TupleA>(tuplea), std::forward<TupleB>(tupleb), std::forward<F>(f),
                   std::make_index_sequence<N>{});
}
*/
template <typename TupleA, typename TupleB, typename F, std::size_t... Indices>
void for_each2_impl_i(TupleA&& tuplea, TupleB&& tupleb, F&& f, std::index_sequence<Indices...>)
{
    using swallow = int[];
    (void)swallow{ 1, (f(std::get<Indices>(std::forward<TupleA>(tuplea)),
                         std::get<Indices>(std::forward<TupleB>(tupleb)), Indices),
                       void(), int{})... };
}

template <typename TupleA, typename TupleB, typename F>
void for_each2_i(TupleA&& tuplea, TupleB&& tupleb, F&& f)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<TupleA>>::value;
    for_each2_impl_i(std::forward<TupleA>(tuplea), std::forward<TupleB>(tupleb), std::forward<F>(f),
                     std::make_index_sequence<N>{});
}
template <typename TupleA, typename F, std::size_t... Indices>
void for_each_impl_i(TupleA&& tuplea, F&& f, std::index_sequence<Indices...>)
{
    using swallow = int[];
    (void)swallow{ 1, (f(std::get<Indices>(std::forward<TupleA>(tuplea)), Indices), void(),
                       int{})... };
}

template <typename TupleA, typename TupleB, typename F>
void for_each_i(TupleA&& tuplea, F&& f)
{
    constexpr std::size_t N = std::tuple_size<std::remove_reference_t<TupleA>>::value;
    for_each_impl_i(std::forward<TupleA>(tuplea), std::forward<F>(f),
                    std::make_index_sequence<N>{});
}

template <size_t index, typename F, typename Tupletuple, std::size_t... Indices>
void for_eachn_impl_impl_impl(F&& f, Tupletuple&& t, std::index_sequence<Indices...>)
{
    f(std::get<index>(std::get<Indices>(t))...), void();
};
template <typename F, typename Tupletuple, size_t index>
void for_eachn_impl_impl(F&& f, Tupletuple&& t)
{
    constexpr size_t N = std::tuple_size<Tupletuple>::value;
    for_eachn_impl_impl_impl<index>(std::forward<F>(f), std::forward<Tupletuple>(t),
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
    constexpr size_t N = std::tuple_size<typename std::remove_reference_t<
      typename std::tuple_element<0, std::remove_reference_t<Tupletuple>>::type>>::value;
    for_eachn_impl(std::forward<F>(f), std::forward<Tupletuple>(t), std::make_index_sequence<N>{});
}

// template <typename F, typename... Tuples>
// void for_eachn(F&& f, Tuples&&... arg)
//{
//    for_eachn(std::forward_as_tuple(arg...), std::forward<F>(f));
//}

// need pop_back of a parameter pack
template <typename... Tuples>
void for_each_tuple(Tuples&&... arg)
{

    auto tup = std::forward_as_tuple(arg...);
    auto slice = tuple_slice<0, sizeof...(Tuples) - 1>(std::forward<std::tuple<Tuples...>>(tup));
    auto f = std::get<sizeof...(Tuples) - 1>(std::forward<std::tuple<Tuples...>>(tup));
    for_eachn<decltype(f), decltype(slice)>(
      // std::forward<decltype(f)>(f),
      std::forward<decltype(slice)>(slice), std::forward<decltype(f)>(f));
};

#endif
