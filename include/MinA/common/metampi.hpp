#include "MinA/common/commonT.hpp"
#include "MinA/common/utility.hpp"
#include <algorithm>
#include <assert.h>
#include <complex>
#include <cstdint>
#include <iostream>
#include <mpi.h>
#include <stdlib.h>
#include <tuple>
#include <type_traits>
#include <vector>
#ifndef MINA_METAMPI
#define MINA_METAMPI
// MPITYPES : A Metaprogramming list of coresponding MPI_Datatypes for c++ types
template <typename... Ts>
struct MPITYPE;

template <>
struct MPITYPE<double> {
 constexpr static MPI_Datatype value = MPI_DOUBLE;
 static auto get() { return value; };
};

template <>
struct MPITYPE<int32_t> {
 constexpr static MPI_Datatype value = MPI_INT;
 static auto get() { return value; };
};

template <>
struct MPITYPE<uint32_t> {
 constexpr static MPI_Datatype value = MPI_UNSIGNED;
 static auto get() { return value; };
};
template <>
struct MPITYPE<uint64_t> {
 constexpr static MPI_Datatype value = MPI_UNSIGNED_LONG;
 static auto get() { return value; };
};

// this is also size_t
template <>
struct MPITYPE<float> {
 constexpr static MPI_Datatype value = MPI_FLOAT;
 static auto get() { return value; };
};
template <>
struct MPITYPE<char> {
 constexpr static MPI_Datatype value = MPI_CHAR;
 static auto get() { return value; };
};
template <typename T>
auto getType(T type)
{
 return MPITYPE<std::remove_reference_t<T>>::get();
}
template <typename... Ts>
struct MPITYPE<std::tuple<Ts...>> {
 static bool set;
 static MPI_Datatype value;
 static auto get()
 {
  if (set)
   return value;
  std::tuple<Ts...> tup;
  constexpr size_t size = std::tuple_size<std::tuple<Ts...>>::value;
  std::array<int, sizeof...(Ts)> block;
  for (auto i = 0; i < sizeof...(Ts); i++) {
   block[i] = 1;
  }
  std::array<MPI_Aint, sizeof...(Ts)> indices;
  std::array<MPI_Datatype, sizeof...(Ts)> types;
  for_each_tuple_i(tup, [&types, &indices, &tup](size_t index, auto& elem) {
   indices[sizeof...(Ts) - 1 - index] = ((long)&elem) - ((long)&tup);
   types[sizeof...(Ts) - 1 - index] =
     MPITYPE<std::remove_reference_t<decltype(elem)>>::get();
  });
  MPI_Datatype new_type;
  MPI_Type_create_struct(size, block.data(), indices.data(), types.data(),
                         &new_type);
  MPI_Type_create_resized(new_type, indices[0], (MPI_Aint)sizeof(tup), &value);
  MPI_Type_free(&new_type);
  set = true;
  return value;
 }
};
template <typename... Ts>
bool MPITYPE<std::tuple<Ts...>>::set(false);
template <typename... Ts>
MPI_Datatype MPITYPE<std::tuple<Ts...>>::value;

template <typename L, typename R>
struct MPITYPE<Result<L, R>> {

 static bool set;
 static MPI_Datatype value;
 static auto get()
 {
  if (set)
   return value;
  std::array<int, 3> block{ 1, 1, 1 };

  std::array<MPI_Aint, 3> indices;
  std::array<MPI_Datatype, 3> types;
  Result<L, R> res;
  indices[0] = (long(&res.value)) - ((long)&res);
  indices[1] = (long(&res.parameters)) - ((long)&res);
  indices[2] = (long(&res.informations)) - ((long)&res);
  types[0] = MPI_DOUBLE;
  types[1] = MPITYPE<L>::get();
  types[2] = MPITYPE<R>::get();
  MPI_Datatype new_type;
  MPI_Type_create_struct(3, block.data(), indices.data(), types.data(),
                         &new_type);
  MPI_Type_create_resized(new_type, indices[0],
                          (MPI_Aint)sizeof(res.value) +
                            (MPI_Aint)sizeof(res.parameters) +
                            (MPI_Aint)sizeof(res.informations),
                          &value);
  MPI_Type_free(&new_type);
  set = true;
  return value;
 }
};
template <typename L, typename R>
bool MPITYPE<Result<L, R>>::set(false);
template <typename L, typename R>
MPI_Datatype MPITYPE<Result<L, R>>::value;

template <typename L>
struct MPITYPE<Result<L, void>> {
 static bool set;
 static MPI_Datatype value;
 static auto get()
 {
  if (set)
   return value;
  std::array<int, 2> block{ 1, 1 };

  std::array<MPI_Aint, 2> indices;
  std::array<MPI_Datatype, 2> types;
  Result<L, void> res;
  indices[0] = (long(&res.value)) - ((long)&res);
  indices[1] = (long(&res.parameters)) - ((long)&res);
  types[0] = MPI_DOUBLE;
  types[1] = MPITYPE<L>::get();
  MPI_Datatype new_type;
  MPI_Type_create_struct(2, block.data(), indices.data(), types.data(),
                         &new_type);
  MPI_Type_create_resized(
    new_type, indices[0],
    (MPI_Aint)sizeof(res.value) + (MPI_Aint)sizeof(res.parameters), &value);
  MPI_Type_free(&new_type);
  set = true;
  return value;
 }
};

template <typename L>
bool MPITYPE<Result<L, void>>::set(false);
template <typename L>
MPI_Datatype MPITYPE<Result<L, void>>::value;

#endif
