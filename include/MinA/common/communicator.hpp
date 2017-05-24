#include "MinA/common/metampi.hpp"
#include "MinA/common/utility.hpp"
#include "mpi.h"
#include <utility>
#ifndef MINA_COMM
#define MINA_COMM
namespace MinA {
template <typename COMTYPE>
class Communicator {
 static std::vector<COMTYPE> cvec;
 int myelement;

 public:
 Communicator(int countx = 0)
 {
  myelement = cvec.size();
  cvec.emplace_back(countx, cvec[myelement - 1]);
 }
 Communicator<COMTYPE> divide(int div)
 {

  Communicator<COMTYPE> cm(std::move(cvec[myelement].divide(div)));
  return cm;
 }
 Communicator(COMTYPE com)
 {
  myelement = cvec.size();
  cvec.push_back(std::move(com));
 }
 ~Communicator() { cvec.pop_back(); }
 bool operator<(decltype(COMTYPE::ident) ident) const
 {
  return cvec[myelement].ident < ident;
 }

 bool operator==(decltype(COMTYPE::ident) ident) const
 {
  return (!(cvec[myelement].ident < ident) && !(ident < cvec[myelement].ident));
 }

 template <typename T>
 void send(decltype(COMTYPE::ident) destination, T t)
 {
  cvec[myelement].send(destination, t);
 };
 template <typename T>
 T receive(decltype(COMTYPE::ident) source)
 {
  return cvec[myelement].receive<T>(source);
 }
 template <typename T>
 void broadcast(T t)
 {
  cvec[myelement].broadcast(t);
 }
 size_t getSize() { return cvec[myelement].size; }
 decltype(COMTYPE::ident) getIdent() { return cvec[myelement].ident; }
 operator bool() const { return (cvec[myelement] == true); }
};
template <typename COMTYPE>
std::vector<COMTYPE> Communicator<COMTYPE>::cvec =
  std::vector<COMTYPE>(1, COMTYPE::getWorldContext());
class MPIContext {
 MPIContext(){};

 public:
 MPIContext(int slice, MPIContext& ctx)
 {
  if (slice > ctx.size) {
   throw std::invalid_argument(std::string(
     std::string("Cannot Split MPI Context:Not Enough Space in MPI Context") +
     std::string("tried to get") + std::to_string(slice) +
     std::string("out of") + std::to_string(ctx.size)));
  }
  if (slice > 0) {
   MPI_Comm_split(ctx.comm, ctx.ident < slice, 0, &comm);
   insplit = (ctx.ident < slice);
  }
  else {
   insplit = ctx.insplit;
   MPI_Comm_dup(ctx.comm, &comm);
  }
  getRankSize();
 }
 MPIContext(MPIContext&&) = default;
 MPIContext(const MPIContext& ref)
 {
  MPI_Comm_dup(ref.comm, &comm);
  insplit = ref.insplit;
  getRankSize();
 }
 ~MPIContext()
 { // MPI_Comm_free(&comm);
 }
 MPI_Comm comm;
 int size;
 int ident; // also rank in mpi
 bool insplit = true;
 void getRankSize()
 {
  MPI_Comm_rank(comm, &ident);
  MPI_Comm_size(comm, &size);
 }
 template <typename Tuple>
 void send(int destination, Tuple tup)
 {
  auto type = MPITYPE<Tuple>::get();
  MPI_Type_commit(&type);
  int err = MPI_Send((&tup), 1, type, destination, 0, comm);
 }
 template <typename Tuple>
 Tuple receive(int source)
 {
  Tuple tup;
  auto type = MPITYPE<Tuple>::get();
  MPI_Type_commit(&type);
  MPI_Status a;
  int err = MPI_Recv(&tup, 1, type, source, 0, comm, &a);
  return tup;
 }
 template <typename Tuple>
 void broadcast(Tuple tup)
 {
  auto type = MPITYPE<Tuple>::get();
  MPI_Type_commit(&type);
  int err = MPI_Bcast(&tup, 1, type, 0, comm);
 }
 MPIContext divide(int divide)
 {
  MPIContext newcon;
  newcon.insplit = true;
  MPI_Comm_split(comm, ident / divide, 0, &newcon.comm);
  newcon.getRankSize();
  return newcon;
 }

 static MPIContext getWorldContext()
 {
  MPI::Init();
  MPIContext com;
  MPI_Comm_dup(MPI_COMM_WORLD, &com.comm);
  com.getRankSize();
  return com;
 }
 operator bool() const { return insplit; }
};
} // namespace MinA
#endif
