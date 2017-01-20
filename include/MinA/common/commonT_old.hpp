#ifndef MINA_COMMONT
#define MINA_COMMONT
#include <tuple>
#include <type_traits>
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
    f(std::get<index>(std::get<Indices>(t))...);
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
#endif
