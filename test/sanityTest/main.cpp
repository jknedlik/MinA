#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
//#include "MinA/algorithm/SimplexParallel.h"
//#include "MinA/common/Minimizer.h"
//#include "MinA/common/Result.h"
//#include "TestFunction.cpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

template <typename Function>
class TestA
  : public MinA::Algorithm<int, std::tuple<double, double>, Function, void> {
 public:
 TestA() { this->mMetaBoundaries = { { 0.0, 2.0 }, { 0.0, 1.0 } }; }
 MinA::Result<typename Function::parametertype, void> run()
 {
  MinA::Result<typename Function::parametertype, void> r;
  r.parameters = this->f.startvalues;
  r.value = this->f.evaluate(this->f.startvalues);
  return r;
 };
 void reset(){};
};

// Himmelblau minimia are at

//  f ( 3.0 , 2.0 ) = 0.0
//    f ( − 2.805118 , 3.131312 ) = 0.0
//    f(-3.779310,-3.283186)=0.0
//  f ( 3.584428 , − 1.848126 ) =00

int main(int argc, char** argv)
{

 TestA<McCormick> t;
 MinA::Simplex<McCormick> s;
 s.setMaxIterations(1000);
 auto r = s.run();
 r.print();

 return (0);
}
