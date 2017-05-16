#ifndef SIMPLEXT_H
#define SIMPLEXT_H
#include "MinA/common/algorithm.hpp"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>
constexpr int columnWidth = 14;

using namespace std;
namespace MinA {
typedef std::tuple<double, double, double, double> SimplexMeta;
#define SIMPLEXT_ALPHA 0
#define SIMPLEXT_BETA 1
#define SIMPLEXT_GAMMA 2
#define SIMPLEXT_TAU 3

#define SIMPLEX_MAX_ITERATIONS 0
#define SIMPLEX_CURR_ITERATIONS 1
#define SIMPLEX_AI_STEPSIZES 2
#define SIMPLEX_AI_CURR_A 3

template <typename Function>
using vertex = std::pair<typename Function::parametertype, double>;
template <typename Function>
using simplext =
  std::array<vertex<Function>,
             std::tuple_size<decltype(Function::startvalues)>::value + 1>;
template <typename Function>
using ai = std::tuple<size_t, size_t, typename Function::parametertype,
                      simplext<Function>>;
template <typename Function>
class Simplex : public MinA::Algorithm<ai<Function>, SimplexMeta, Function> {
 public:
 static constexpr size_t mDimension =
   std::tuple_size<typename Function::parametertype>::value;
 Simplex() : MinA::Algorithm<ai<Function>, SimplexMeta, Function>::Algorithm()
 {
  reset();
 }
 virtual void reset()
 {
  this->restored = false;
  this->mMetaBoundaries = decltype(this->mMetaBoundaries){
   { 0.0, 2.0 }, { 0.0, 1.0 }, { 0.0, 2.0 }, { 0.0, 1.0 }
  };
  this->mMetaParameters = decltype(this->mMetaParameters){ 1.0, 0.5, 1.0, 0.5 };
  std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations) = 0;
  std::get<SIMPLEX_MAX_ITERATIONS>(this->mAlgorithmInformations) = 100;
 }
 ~Simplex() { ; }

 void initializeVertices(simplext<Function>& A)
 {
  // Initialize the all vertices to the starting parameters
  for (size_t i = 0; i < mDimension + 1; i++) {
   A[i].first =
     TranslateVal<mDimension>(this->f.startvalues, this->f.bounds, false);
  }

  for (size_t i = 1; i < mDimension + 1; i++) {
   // iterate over tuples and step sizes,
   // Change ith vertex's by adding i-th stepSize in i-th direction to A[0]
   for_each_tuple_i(
     A[i].first, std::get<SIMPLEX_AI_STEPSIZES>(this->mAlgorithmInformations),
     [&i](size_t index, auto& vex, auto step) {
      if (i - 1 == index) {
       vex = vex + step;
      }
     });
  }
 }
 void setMaxIterations(size_t i)
 {
  std::get<SIMPLEX_MAX_ITERATIONS>(this->mAlgorithmInformations) = i;
 }
 void setAlpha(double a)
 {
  std::get<SIMPLEXT_ALPHA>(this->mMetaParameters) = a;
 }
 void setBeta(double b) { std::get<SIMPLEXT_BETA>(this->mMetaParameters) = b; }

 void setTau(double t) { std::get<SIMPLEXT_TAU>(this->mMetaParameters) = t; }
 void setGamma(double g)
 {
  std::get<SIMPLEXT_GAMMA>(this->mMetaParameters) = g;
 }
 vertex<Function> getCentroid(simplext<Function> A)
 {
  // Return centroid M of all vertices, but the nExcluded worst ones, that is
  // the centroid of the best (mDimension + 1 - nExcluded) vertices of the
  // vector of vertices A In the serial case the centroid is calculated on
  // mDimension vertices, that is all but the worst one. The vertices of A are
  // supposed to be already ordered by function value, from best to worst
  //
  vertex<Function> M;
  constexpr size_t msize =
    std::tuple_size<typename Function::parametertype>::value;

  for (int iVertex = 0; iVertex < msize + 1 - this->mpi_procs; ++iVertex) {

   for_each_tuple(M.first, A[iVertex].first, [](auto& l, auto r) { l += r; });
  }
  // for_each_tuple_i(M.first, [this](auto l, size_t num) { l /= (msize + 1 -
  // (this->mpi_procs));
  // });
  for_each_tuple(
    M.first, [this](auto& l) { l /= (mDimension + 1 - (this->mpi_procs)); });
  this->checkBoundaryCondition(M.first);

  M.second =
    this->f.evaluate(TranslateVal<mDimension>(M.first, this->f.bounds, true));
  return M;
 }
 vertex<Function> getReflectedPoint(vertex<Function>& M, vertex<Function>& Aj)
 {
  // Return reflection point Ar of initial point Aj w.r.t. centroid M of
  // remaining points Ar = M + alpha * (M - Aj)
  //
  vertex<Function> Ar;
  // Ar.first.resize(mDimension);

  for_each_tuple(M.first, Ar.first, Aj.first, [&](auto m, auto& ar, auto aj) {
   ar = m + std::get<SIMPLEXT_ALPHA>(this->mMetaParameters) * (m - aj);
  });
  this->checkBoundaryCondition(Ar.first);
  Ar.second =
    this->f.evaluate(TranslateVal<mDimension>(Ar.first, this->f.bounds, true));
  return Ar;
 }

 vertex<Function> getExtendedPoint(vertex<Function>& M, vertex<Function>& Ar)
 {
  // Return extension point Ae of initial point Ar w.r.t. centroid M
  // Ae = Ar + gamma * (Ar - M)
  //
  vertex<Function> Ae;
  for_each_tuple(M.first, Ar.first, Ae.first, [&](auto m, auto ar, auto& ae) {
   ae = ar + std::get<SIMPLEXT_GAMMA>(this->mMetaParameters) * (ar - m);
  });
  this->checkBoundaryCondition(Ae.first);
  Ae.second =
    this->f.evaluate(TranslateVal<mDimension>(Ae.first, this->f.bounds, true));
  return Ae;
 }
 vertex<Function> getContractedPoint(vertex<Function>& M, vertex<Function>& Ajp)
 {
  // Return contraction point Ac of initial point Ajp w.r.t. centroid M
  // Ac = M + beta * (Ajp - M)
  //
  vertex<Function> Ac;
  for_each_tuple(M.first, Ajp.first, Ac.first, [&](auto m, auto ajp, auto& ac) {
   ac = m + std::get<SIMPLEXT_BETA>(this->mMetaParameters) * (ajp - m);
  });

  this->checkBoundaryCondition(Ac.first);
  Ac.second =
    this->f.evaluate(TranslateVal<mDimension>(Ac.first, this->f.bounds, true));
  return Ac;
 }
 vertex<Function> getShrinkedPoint(vertex<Function>& Ap, vertex<Function>& A0)
 {
  // Return shrinked point Anew of initial point Ap w.r.t. fixed (best) point A0
  // Anew = tau * A0 + (1-tau) * Ap
  //
  vertex<Function> Anew;
  for_each_tuple(
    Ap.first, A0.first, Anew.first, [&](auto ap, auto a0, auto& anew) {
     anew = std::get<SIMPLEXT_TAU>(this->mMetaParameters) * a0 +
            (1 - std::get<SIMPLEXT_TAU>(this->mMetaParameters)) * ap;
    });
  this->checkBoundaryCondition(Anew.first);
  // Anew.second = this->f.evaluate(Anew.first);
  return Anew;
 }

 double getSimplexSize(simplext<Function>& A)
 {
  double simplexSize = 0.;

  vertex<Function> M =
    getCentroid(A); // the centroid of the simplex, no vertex excluded
  for (int i = 0; i < A.size(); i++) {
   double vertexCentroidDistance = 0.;
   for_each_tuple(A[i].first, M.first, [&](auto a, auto m) {
    vertexCentroidDistance += pow(m - a, 2);
   });
   simplexSize += sqrt(vertexCentroidDistance);
  }
  return simplexSize / A.size();
 }

 void setSize()
 {
  for_each_tuple(
    std::get<SIMPLEX_AI_STEPSIZES>(this->mAlgorithmInformations),
    this->f.startvalues, this->f.bounds,
    [](auto& step, auto& start, auto bound) {
     if (std::abs(start - bound.left) > std::abs(bound.right - start))
      step = start - bound.left;
     else
      step = bound.right - start;
     step *= 0.8;
    });
 };
 void setStepSize(typename Function::parametertype& s)
 {
  std::get<SIMPLEX_AI_STEPSIZES>(this->mAlgorithmInformations) = s;
 }

 void printOutVertices(simplext<Function>& simplexVertices, ostream& outStream)
 {
  // Print out parameter values and function value at each vertex

  for (int iVertex = 0; iVertex <= mDimension; ++iVertex) {
   string vertexName{ "A[" };
   vertexName += to_string(iVertex);
   vertexName += "]";
   printOutVertex(simplexVertices[iVertex], vertexName, outStream);
  }
 }

 void printOutVertex(vertex<Function>& simplexVertex, string vertexName,
                     ostream& outStream)
 {
  // Print out parameters and function value at vertex
  // outStream << setw(columnWidth - 2) << vertexName << ":  ";
  outStream << setw(columnWidth) << vertexName;
  outStream << setprecision(7);
  auto r = TranslateVal<mDimension>(simplexVertex.first, this->f.bounds, true);
  for_each_tuple(
    r, [&outStream](auto x) mutable { outStream << setw(columnWidth) << x; });

  outStream << "   f(" << vertexName << ")= " << setw(columnWidth)
            << simplexVertex.second << "\n";
 }
 bool checkStoppingCondition()
 {
  auto max = std::get<SIMPLEX_MAX_ITERATIONS>(this->mAlgorithmInformations);
  auto cur = std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations);

  std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations) += 1;
  return (cur < max);
 }

 virtual Result<typename Function::parametertype> run()
 {
  constexpr size_t mDimension =
    std::tuple_size<typename Function::parametertype>::value;
  setStepSize();
  simplext<Function> A;

  this->filename = this->filename + ".simplex";
  this->restore();
  if (!this->restored) {
   initializeVertices(A); // Initial
  }
  else {
   A = std::get<SIMPLEX_AI_CURR_A>(this->mAlgorithmInformations);
  }
  string outFile_fValue(this->filename + "_fValue");
  ofstream fValueFile;
  fValueFile.open(outFile_fValue, ios::app);
  for_each_tuple(this->mMetaParameters,
                 [&fValueFile](auto x) { fValueFile << x << "|"; });
  fValueFile << std::endl;
  fValueFile.close();

  for (int i = 0; i <= mDimension; i++)
   this->checkBoundaryCondition(A[i].first);
  while (checkStoppingCondition()) {
   vertex<Function> Ar, Ac, Ae, Anew,
     Ap; // reflection, contraction, extension, shrinked point
   for (int iVertex = 0; iVertex <= mDimension; ++iVertex) {
    auto e = TranslateVal<mDimension>(A[iVertex].first, this->f.bounds, true);
    A[iVertex].second = this->f.evaluate(e);
   }
   sort(A.begin(), A.end(),
        [](vertex<Function>& a, vertex<Function>& b) -> bool {
         return a.second < b.second;
        }); // Sort
   ofstream verticesFile;
   string outFile_vertices(this->filename + "_Vertices");
   verticesFile.open(outFile_vertices, ios::app);
   verticesFile << "  Iteration: "
                << std::get<SIMPLEX_CURR_ITERATIONS>(
                     this->mAlgorithmInformations)
                << endl;
   printOutVertices(A, verticesFile);
   verticesFile << endl;
   verticesFile.close();

   // get centroid of all vertices but the worst one
   vertex<Function> M = getCentroid(A);

   int check = 0;
   Ar = getReflectedPoint(M, A[mDimension]);
   if (Ar.second < A[0].second) {
    Ae = getExtendedPoint(M, Ar);
    if (Ae.second < A[0].second) {
     Anew = Ae;
     check = 1;
    }
    else {
     Anew = Ar;
     check = 2;
    }
   } // case 1
   else {
    if (Ar.second < A[mDimension - 1].second) {
     Anew = Ar;
     check = 3;
    } // case 2
    else {
     if (Ar.second < A[mDimension].second) {
      Ap = Ar;
     }
     else {
      Ap = A[mDimension];
     } // Ac
     Ac = getContractedPoint(M, Ap);
     if (Ac.second < A[mDimension].second) {
      Anew = Ac;
      check = 4;
     }
    } // case 3
   }
   if (check > 0) {
    A[mDimension] = Anew;
   }
   else {
    for (int i = 1; i <= mDimension; i++) {
     Anew = getShrinkedPoint(A[i], A[0]);
     Anew.second = this->f.evaluate(
       TranslateVal<mDimension>(Anew.first, this->f.bounds, true));
     A[i] = Anew;
    }
   }

   this->checkBoundaryCondition(A[mDimension].first);
   std::get<SIMPLEX_AI_CURR_A>(this->mAlgorithmInformations) = A;
   this->save();

   string outFile_fValue(this->filename + std::string("_fValue"));
   fValueFile.open(outFile_fValue, ios::app);
   fValueFile << "Iteration " << setw(5)
              << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
              << "   ";
   printOutVertex(A[0], "A[0]", fValueFile);
   fValueFile.close();
  }
  MinA::Result<typename Function::parametertype> r;
  r.parameters = TranslateVal<mDimension>(A[0].first, this->f.bounds, true);
  r.value = A[0].second;
  return r;
 }
};
} // namespace MinA
#endif
