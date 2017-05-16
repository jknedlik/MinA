#include "MinA/common/commonT.hpp"
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
};

template <>
struct MPITYPE<int32_t> {
 constexpr static MPI_Datatype value = MPI_INT;
};

template <>
struct MPITYPE<uint32_t> {
 constexpr static MPI_Datatype value = MPI_UNSIGNED;
};
template <>
struct MPITYPE<uint64_t> {
 constexpr static MPI_Datatype value = MPI_UNSIGNED_LONG;
};

// this is also size_t
template <>
struct MPITYPE<float> {
 constexpr static MPI_Datatype value = MPI_FLOAT;
};
template <>
struct MPITYPE<char> {
 constexpr static MPI_Datatype value = MPI_CHAR;
};

template <size_t index, typename... Ts>
struct MPITypes;
template <size_t index, typename T, typename... Ts>
struct MPITypes<index, T, Ts...> {
 static void fill(decltype(MPI_DOUBLE) * types)
 {
  types[index] = MPITYPE<T>::value;
  MPITypes<index + 1, Ts...>::fill(types);
 }
};
template <size_t index, typename T>
struct MPITypes<index, T> {
 static void fill(decltype(MPI_DOUBLE) * types)
 {
  types[index] = MPITYPE<T>::value;
 }
};

template <size_t index, typename Tuple>
void fillarr(Tuple& tup, MPI_Aint* arr, MPI_Datatype* darr,
             std::false_type::type)
{
 auto max = arr[std::tuple_size<Tuple>::value - 1];
 for (size_t i = 0; i < std::tuple_size<Tuple>::value; i++) {
  arr[i] = abs(max - arr[i]);
 }
};

template <size_t index, typename Tuple>
void fillarr(Tuple& tup, MPI_Aint* arr, MPI_Datatype* darr,
             std::true_type::type)
{
 auto a = (long)(&std::get<index>(tup));
 auto b = (long)(&std::get<0>(tup));
 darr[index] =
   MPITYPE<std::remove_reference_t<decltype(std::get<index>(tup))>>::value;
 arr[index] = b - a;
 fillarr<index + 1, Tuple>(
   tup, arr, darr,
   typename std::integral_constant<
     bool, (index + 1 < std::tuple_size<Tuple>::value)>::type());
}

template <typename Tuple>
void fillarr(Tuple& tup, MPI_Aint* arr, MPI_Datatype* darr)
{
 fillarr<0, Tuple>(tup, arr, darr, std::true_type::type());
};

template <typename Tuple>
MPI_Datatype createMPIType(Tuple& tup)
{
 constexpr size_t size = std::tuple_size<Tuple>::value;
 MPI_Datatype new_type;
 std::array<MPI_Aint, size> indices;
 std::array<MPI_Datatype, size> mpi_elements;
 std::array<int, size> block;
 for (auto i = 0; i < size; i++) {
  block[i] = 1;
 }
 fillarr(tup, indices.data(), mpi_elements.data());
 std::reverse(indices.begin(), indices.end());
 std::reverse(mpi_elements.begin(), mpi_elements.end());
 MPI_Type_create_struct(size, block.data(), indices.data(), mpi_elements.data(),
                        &new_type);
 MPI_Datatype resized;
 MPI_Type_create_resized(new_type, indices[0], (MPI_Aint)sizeof(tup), &resized);
 MPI_Type_free(&new_type);
 return resized;
}

template <typename Tuple>
Tuple mpi_message(Tuple& tup, int source, int destination,
                  auto comm = MPI_COMM_WORLD, int tag = 0)
{
 int rank;
 auto type = createMPIType(tup);
 MPI_Type_commit(&type);
 MPI_Comm_rank(comm, &rank);

 if (rank == source) {
  for_each_tuple(tup, [](auto x) { std::cout << x << " "; });
  int err = MPI_Send((&tup), 1, type, destination, tag, comm);
 }
 if (rank == destination) {
  MPI_Status a;
  int err = MPI_Recv(&tup, 1, type, source, tag, comm, &a);
  for_each_tuple(tup, [](auto x) { std::cout << x << " "; });
 }
 return tup;
}
#endif
