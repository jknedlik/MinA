#ifndef MINA_PSIMPLEX
#define MINA_PSIMPLEX
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/communicator.hpp"
#include "MinA/common/utility.hpp"
namespace MinA {
template <typename Function>
class ParallelSimplex : public Simplex<Function> {
 private:
 public:
 using vertex = std::pair<typename Function::parametertype, double>;
 using simplext =
   std::array<vertex,
              std::tuple_size<decltype(Function::startvalues)>::value + 1>;
 using ai =
   std::tuple<size_t, size_t, typename Function::parametertype, simplext>;
 static constexpr size_t mDimension =
   std::tuple_size<typename Function::parametertype>::value;
 ParallelSimplex() : Simplex<Function>(){};

 Result<typename Function::parametertype, ai> run()
 {

  constexpr size_t mDimension =
    std::tuple_size<typename Function::parametertype>::value;
  if (this->mpi_procs > mDimension + 1) {
   throw std::runtime_error(
     " simplex of size" + (std::to_string(mDimension + 1)) +
     "cannot be calculated by " + std::to_string(this->mpi_procs) + "cores");
  }
  simplext A;
  this->setStepSize();
  this->filename += ".psimplex";
  this->restore();
  bool initialized = this->restored;
  if (!this->restored) {
   this->initializeVertices(A); // Initialze A
  }
  else {
   A = std::get<SIMPLEX_AI_CURR_A>(this->mAlgorithmInformations);
  }

  MinA::Communicator<MinA::MPIContext> all(this->mpi_procs * this->f.mpi_procs);
  if (all) {
   if (all == 0) {
    int mode;

    while (this->checkStoppingCondition()) {
     // Initial evaluation
     if (!initialized) {
      evalSimplex(A, all);
      initialized = true;
     }
     ofstream verticesFile;
     string outFile_vertices(this->filename +
                             ".proc:" + std::to_string(all.getIdent()) +
                             to_string(all.getSize()) + "_Vertices");
     verticesFile.open(outFile_vertices, ios::app);
     verticesFile << "  Iteration: "
                  << std::get<SIMPLEX_CURR_ITERATIONS>(
                       this->mAlgorithmInformations)
                  << endl;
     this->printOutVertices(A, verticesFile);
     verticesFile << endl;
     verticesFile.close();

     // Centroid of the simplex, excluding world_size vertices
     vertex M = this->getCentroid(A);

     ofstream meanFile;
     string outFile_mean(this->filename + ".proc:" + to_string(all.getSize()) +
                         "_SimplexSize");
     meanFile.open(outFile_mean, ios::app);
     meanFile << "Iteration= "
              << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
              << "	Mean= " << M.second
              << "	SimplexSize =	" << this->getSimplexSize(A) << "	";
     meanFile << endl;
     meanFile.close();
     mode = 1;
     for (int i = 1; i < all.getSize(); i++) {
      sendMode(1, all, i);
      sendVertex(M, i, all);
      sendVertex(A[0], i, all);
      sendVertex(A[mDimension - all.getSize() + i], i, all);
      sendVertex(A[mDimension - all.getSize() + i + 1], i, all);

     } // sending M,A0,Aj_1,Aj
       // calc your own vertex
     auto ret = clientRun(M, A[mDimension - all.getSize() + 1],
                          A[mDimension - all.getSize()], A[0]);

     int sumcheck = std::get<0>(ret);

     for (int i = 1; i < all.getSize(); i++) {
      sumcheck += receiveMode(i, all);
     }
     if (sumcheck == 0) {
      for (int i = 1; i < all.getSize(); i++) {
       A[mDimension - all.getSize() + i + 1] =
         this->getShrinkedPoint(A[mDimension - all.getSize() + i + 1], A[0]);
      }
      for (int i = 1; i < all.getSize(); i++) {
       vertex b;
       b = receiveVertex(i, all);
       A[mDimension - all.getSize() + i + 1] = this->getShrinkedPoint(b, A[0]);
      }
      A[mDimension - all.getSize()] =
        this->getShrinkedPoint(std::get<1>(ret), A[0]);
      evalSimplex(A, all);
     } // case 4
     else {
      for (int i = 1; i < all.getSize(); i++) {
       A[mDimension - all.getSize() + i + 1] = receiveVertex(i, all);
      }
      A[mDimension - all.getSize() + 1] = std::get<1>(ret);
     }
     sort(A.begin(), A.end(),
          [](vertex& a, vertex& b) -> bool { return a.second < b.second; });
     ofstream fValueFile;
     string outFile_fValue(this->filename +
                           ".proc:" + to_string(all.getSize()) + "_fValue");
     fValueFile.open(outFile_fValue, ios::app);
     fValueFile << "Iteration= "
                << std::get<SIMPLEX_CURR_ITERATIONS>(
                     this->mAlgorithmInformations)
                << "   ";
     this->printOutVertex(A[0], "A[0]", fValueFile);
     fValueFile.close();
     // save();
    }

    mode = 0;

    MinA::Result<typename Function::parametertype, ai> r;
    r.parameters = A[0].first;
    r.value = A[0].second;
    for (int i = 1; i < all.getSize(); i++) {
     sendMode(0, all, i);
     sendVertex(A[0], i, all);
    }
    // Result
    return r;
   }

   else {

    while (1) {
     int mode = receiveMode(0, all);
     if (mode == 1) {
      vertex M, A0, Aj, Aj_1;
      M = receiveVertex(0, all);
      A0 = receiveVertex(0, all);
      Aj_1 = receiveVertex(0, all);
      Aj = receiveVertex(0, all);

      auto ret = clientRun(M, Aj, Aj_1, A0);

      sendMode(std::get<0>(ret), all, 0);
      sendVertex(std::get<1>(ret), 0, all);
     }

     if (mode == 0) {

      MinA::Result<typename Function::parametertype, ai> r;
      vertex v = receiveVertex(0, all);
      r.parameters = v.first;
      r.value = v.second;
      return r;
     }
     if (mode == 2) {
      vertex v = receiveVertex(0, all);
      v.second =
        this->f(TranslateVal<mDimension>(v.first, this->f.bounds, true));

      sendVertex(v, 0, all);
     }
    }
   }
  }
  // real ugly leftover from v1, needs to be refactored

  MinA::Result<typename Function::parametertype, ai> r;
  return r;
 }

 vertex getContractedPoint(vertex& M, vertex& Ajp)
 {
  // Return contraction point Ac of initial point Ajp w.r.t. centroid M
  // Ac = beta * (Ajp + M)
  //
  vertex Ac;

  for_each_tuple(
    Ac.first, Ajp.first, M.first, [this](auto& ac, auto& ajp, auto& m) {
     ac = std::get<SIMPLEXT_BETA>(this->mMetaParameters) * (ajp + m);
    });

  Ac.second = this->f(TranslateVal<mDimension>(Ac.first, this->f.bounds, true));
  return Ac;
 }
 void sendVertex(vertex& v, int u_id, MinA::Communicator<MinA::MPIContext>& all)
 {
  all.send(u_id, v.first);
  all.send(u_id, std::make_tuple(v.second));
 }
 vertex receiveVertex(int u_id, MinA::Communicator<MinA::MPIContext>& all)
 {
  vertex v;
  v.first = all.receive<decltype(v.first)>(u_id);
  auto x = all.receive<std::tuple<double>>(u_id);
  v.second = std::get<0>(x);
  return v;
 }
 void sendMode(int i, MinA::Communicator<MinA::MPIContext>& all, int u_id)
 {
  all.send(u_id, std::make_tuple(i));
 }
 int receiveMode(int u_id, MinA::Communicator<MinA::MPIContext>& all)
 {

  std::tuple<int> x = all.receive<std::tuple<int>>(u_id);

  return std::get<0>(x);
 }

 std::tuple<int, vertex> clientRun(vertex& M, vertex& Aj, vertex& Aj_1,
                                   vertex& A0)
 {
  vertex Ar, Ac, Ae, Ap;
  int check = 0;

  // Update vertex step3
  Ar = this->getReflectedPoint(M, Aj);
  if (Ar.second < A0.second) {
   Ae = this->getExtendedPoint(M, Ar);
   if (Ae.second < A0.second) {
    Aj = Ae;
    check = 1;
   }
   else {
    Aj = Ar;
    check = 2;
   }
  } // case 1
  else if (Ar.second < Aj_1.second) {
   Aj = Ar;
   check = 3;
  } // case 2
  else {
   if (Ar.second < Aj.second) {
    Ap = Ar;
   }
   else {
    Ap = Aj;
   } // Ac
   Ac = this->getContractedPoint(M, Ap);

   if (Ac.second < Ap.second) {
    Aj = Ac;
    check = 4;
   } // case 3
   else
    Aj = Ap;
  }

  return std::make_tuple(check, Aj);
 }

 void evalSimplex(simplext& A, MinA::Communicator<MinA::MPIContext>& all)
 {
  for (int i = A.size(); i >= 0; i--) {
   if (i % all.getSize() == 0)
    A[i - 1].second =
      this->f(TranslateVal<mDimension>(A[i - 1].first, this->f.bounds, true));
   else {
    sendMode(2, all, i % all.getSize());
    sendVertex(A[i - 1], i % all.getSize(), all);
   }
  }
  for (int i = A.size(); i > 0; i--) {
   if (i % all.getSize() != 0)
    A[i - 1] = receiveVertex(i % all.getSize(), all);
  }
  sort(A.begin(), A.end(),
       [](vertex& a, vertex& b) -> bool { return a.second < b.second; });
 }
};
} // namespace MinA
#endif
