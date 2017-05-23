#ifndef GSLSIMPLEXT_H
#define GSLSIMPLEXT_H
#include "MinA/common/algorithm.hpp"
#include "gsl/gsl_multimin.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <cstdio>
#include <fstream>
#include <gsl/gsl_eigen.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_sf_bessel.h>
#include <gsl/gsl_vector.h>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <utility>

using namespace std;
namespace MinA {

template <typename Function>
class GSLSimplex
  : public MinA::Algorithm<std::tuple<int>, std::tuple<int>, Function, void> {
 public:
 static constexpr size_t mDimension =
   std::tuple_size<typename Function::parametertype>::value;

 GSLSimplex()
   : MinA::Algorithm<std::tuple<int>, std::tuple<int>, Function,
                     void>::Algorithm()
 {
  reset();
 }
 virtual void reset() { this->restored = false; }
 ~GSLSimplex() { ; }
 static double evalGSL(const gsl_vector* v, void* params)
 {
  Function f;
  typename Function::parametertype inparam;
  for_each_tuple_i(
    inparam, [&v](size_t index, auto& ip) { ip = gsl_vector_get(v, index); });
  return f(inparam);
 }

 virtual typename Function::result run()
 {

  // GSL-stuff
  int maxNumberOfIteration = 100;
  size_t iter = 0;
  int status;
  double size;

  // function
  gsl_multimin_function my_func;
  my_func.n = mDimension; /* number of function components */
  my_func.f = &MinA::GSLSimplex<Function>::evalGSL;

  // arguments
  gsl_vector* x = gsl_vector_alloc(mDimension);

  // Startwerte
  // gsl_vector_set_all (x, 0);
  for (int k = 0; k < mDimension; k++)

   for_each_tuple_i(this->f.startvalues, [&x](size_t index, auto& v) {
    gsl_vector_set(x, index, v);
   });

  // step size
  gsl_vector* stepSize = gsl_vector_alloc(mDimension);
  gsl_vector_set_all(stepSize, 0.1);

  // initialize the minimizer

  const gsl_multimin_fminimizer_type* T = gsl_multimin_fminimizer_nmsimplex2;
  gsl_multimin_fminimizer* minimizer =
    gsl_multimin_fminimizer_alloc(T, mDimension);

  /*if (MATHCODE(FitStrategy) == 1) {
   const gsl_multimin_fminimizer_type* T = gsl_multimin_fminimizer_nmsimplex2;
   gsl_multimin_fminimizer* minimizer =
     gsl_multimin_fminimizer_alloc(T, mDimension);
  };

  if (MATHCODE(FitStrategy) == 2) {
   const gsl_multimin_fminimizer_type* T =
     gsl_multimin_fminimizer_nmsimplex2rand;
   gsl_multimin_fminimizer* minimizer =
     gsl_multimin_fminimizer_alloc(T, mDimension);
  };*/

  // 	time_t start = clock();
  iter = 0;
  gsl_multimin_fminimizer_set(minimizer, &my_func, x, stepSize);
  do {
   iter++;
   status = gsl_multimin_fminimizer_iterate(minimizer);

   if (status)
    break;

   size = gsl_multimin_fminimizer_size(minimizer);
   // std::cout << "fval=" << minimizer->fval << std::endl;
   status = gsl_multimin_test_size(size, 1e-3);
  } while (status == GSL_CONTINUE && iter < maxNumberOfIteration);

  // Werte zurück schreiben

  typename Function::parametertype outparam;
  for_each_tuple_i(outparam, [&minimizer](size_t index, auto& ip) {
   ip = gsl_vector_get(minimizer->x, index);
  });
  typename Function::result r;
  r.value = minimizer->fval;
  r.parameters = outparam;

  //	time_t end = clock();
  //	double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
  //	std::cout<<"cpu time used  "<< cpu_time_used << "s" << " for "<< iter << "
  // iterations"<<std::endl;

  gsl_multimin_fminimizer_free(minimizer);
  gsl_vector_free(x);
  gsl_vector_free(stepSize);

  return r;
 }
}; // namespace MinA
} // namespace MinA
#endif
