#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/Multi.h"
#include "MinA/algorithm/ParallelSimplex.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "MinA/common/communicator.hpp"
#include "mpi.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <utility>
using namespace std;

template <size_t N>
class NTest : public MinA::Function<typename TArray<double, N>::type> {
 public:
 NTest() : MinA::Function<typename TArray<double, N>::type>::Function() {}
 double evaluate(typename TArray<double, N>::type params)
 {
  std::vector<double> vec;
  for_each_tuple(params, [&vec](auto val) { vec.push_back(val); });

  double x = 0;
  for (auto v : vec) {
   x += v;
  }
  return x;
 }
};

template <size_t N, template <size_t> typename T,
          template <typename> typename P>
struct seqqer : seqqer<N - 1, T, P> {
 using basetype = seqqer<N - 1, T, P>;
 using ltype = std::tuple<P<T<N>>>;
 using type = decltype(std::tuple_cat(typename basetype::type(), ltype()));
};
template <template <size_t> typename T, template <typename> typename P>
struct seqqer<1, T, P> {
 using type = std::tuple<P<T<1>>>;
};

template <size_t N>
struct seq2 : seq2<N - 1> {
 using S = MinA::Simplex<NTest<N>>;
 using type =
   decltype(std::tuple_cat(typename seq2<N - 1>::type(), std::tuple<S>()));
};
template <>
struct seq2<1> {
 using type = std::tuple<MinA::Simplex<NTest<1>>>;
};

template <size_t... Is>
struct seq3 {
 using type = std::tuple<MinA::Simplex<NTest<Is>>...>;
};
int main(int argc, char** argv)
{

 // seqqer<22, NTest, MinA::Simplex>::type allS;
 // seq2<22>::type allS;
 // std::index_sequence<0, 11, 20, 100>;
 // seq3<1, 11, 20, 100>::type allS;
 seq3<1, 2, 3, 4, 20, 40>::type allS;
 // std::tuple<MinA::Simplex<NTest<11>>> allS;
 std::vector<double> t{
  0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0
 };
 for_each_tuple(allS, [t](auto& s) {
  if (std::tuple_size<typename decltype(s.f)::parametertype>::value ==
      t.size()) {
   for_each_tuple_i(s.f.startvalues, [t](size_t j, auto& sv) {
    std::cout << "setting startvalues i:" << j << "to" << t[j] << std::endl;
    sv = t[j];
   });
   for_each_tuple_i(s.f.bounds, [t](size_t j, auto& b) {
    b.left = 0;
    b.right = 14;
   });

   s.setMaxIterations(10000);
   auto r = s.run();

   r.print();
  }
 });
 //  MPI::Finalize();
 return (0);
}
