#include "MinA/common/algorithm.hpp"
#include "MinA/common/metampi.hpp"
#include "MinA/common/utility.hpp"
#include <cstddef>
#include <experimental/tuple>
#include <functional>
#include <iostream>
#include <limits>
#include <numeric>
#include <tuple>
#include <utility>
#include <vector>
using namespace MinA;
template <typename T>
void printmax(T t)
{
 std::cout << std::numeric_limits<T>::min() << std::endl;
}

std::tuple<int, int, double> t;
template <typename... Ts>
void tfy(std::tuple<Ts...> tup, std::array<int, std::tuple_size<Ts...>::value>)
{
 // std::stack < std::tuple < Ts... >> stack;

 for (int i = 0; i < std::tuple_size<Ts...>::value; i++) {
 }
}

template <typename Tuple>
struct Vectortuple {
 using type = std::vector<Tuple>;
};

std::tuple<std::tuple<int, double>, std::tuple<int, double>> megat;

struct Tester {
 int cool = 5;
 int shit = 4;
};

class myfunc : Function<double, double, int> {
 public:
 myfunc(){};
 double evaluate(std::tuple<double, double, int> tup)
 {
  return std::get<0>(tup) * std::get<1>(tup) * std::get<2>(tup);
 }
};
int main()
{
 std::tuple<double, int> xr{ 5.9, 8 };
 std::tuple<int, int> yr{ 10, 0 };

 for_each_tuple(xr, yr, [](auto& xx, auto& yy) {
  xx *= 10;
  yy *= 10;
 });

 for_eachn(std::tie(xr, yr), [](auto& xx, auto& yy) {
  std::cout << xx << " " << yy << std::endl;
 });

 //  for_each_tuple(xr, yr, [](auto& tuple, auto& t2) { std::cout << tuple <<
 //  t2; });
 // for_each_tuplen(xr, yr, [](auto& xx, auto& yy) { std::cout << xx << " " <<
 // yy << std::endl;
 // });

 myfunc f;
 std::tuple<double, double, int> gr{ 2, 4, 4 };
 std::cout << "f eval:" << f.evaluate(gr) << "\n" << std::endl;
 /*std::function<double(std::tuple<double, double, double> &)> f = [](auto
 t) {
   return std::get<0>(t) * std::get<1>(t) * std::get<2>(t);
 };
 Function<double, double, double> func(f);
 auto t = std::make_tuple(2.0, 3.0, 4.0);
 std::cout << "func eval:" << func.evaluate(t) << std::endl;
 */
 // Function<double,double,double>

 std::tuple<double, double, int> testt;
 std::tuple<double, double, int> testtt;

 // for_each_tuple(t, [](auto x) { std::cout << x; });

 for_each_tuple(testt, testtt, [](auto& t, auto& s) {
  std::cout << t << "ttt" << s << std::endl;
 });
 Boundarytuple<decltype(testt)>::type testboundary;
 for_each_tuple(testboundary, [&](auto& b) {
  b.left = 0;
  b.right = 1;
 });
 for_each_tuple(testboundary,
                [&](auto b) { std::cout << b.left << "<-l r->" << b.right; });

 for_each_tuple(megat,
                [](auto x) { for_each_tuple(x, [](auto y) { printmax(y); }); });
 TupleVector<std::tuple<int, int, double>>::type x{
  { 11, 12, 13, 14 }, { 21 }, { 31.5, 32.5 }
 };
 TupleVector<int, std::string>::type z;
 std::get<0>(z).push_back(154);
 std::get<1>(z).push_back("hi");
 std::get<1>(z).push_back("pause");
 auto a = *(std::get<0>(z).begin());
 std::cout << "a " << a << std::endl;
 std::cout << "x" << std::endl;
 auto y = x;
 auto b = std::get<0>(y).rbegin();
 // std::cout << *b << std::endl;
 std::cout << std::tuple_size<decltype(x)>::value << std::endl;
 std::vector<std::tuple<int, int, double>> tv;
 constructTuple(x, [&tv](auto t) { tv.push_back(t); });
 std::cout << "ctupletest";
 for (auto tvi : tv) {
  for_each_tuple(tvi, [](auto x) { std::cout << x; });
 }
 std::cout << std::endl;
 // constructTuple(z);

 // auto type = createMPIType<int, int, double>();
 Tester tester;

 MPI::Init();
 std::tuple<int, int, int, int> ttt{ 1, 2, 3, 4 };
 int rank;
 ;
 std::cout << MPI_INT << "i" << MPI_DOUBLE << "d" << MPI_CHAR << "c"
           << std::endl;
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 std::tuple<double, int, double, int, int, int, int> mpitup;
 std::cout << "Mpitupsize: " << sizeof(mpitup) << std::endl;
 std::cout << "Char,Double,int" << MPI_CHAR << " " << MPI_DOUBLE << " "
           << MPI_INT << std::endl;

 for (int i = 0; i < 1; i++) {
  if (rank == 0) {
   std::tuple<double, int, double, int, int, int, int> mptup{
    std::numeric_limits<double>::max(),
    2,
    std::numeric_limits<double>::max(),
    3,
    4,
    5,
    6
   };
   mpitup = mpi_message(mptup, 0, 1, MPI_COMM_WORLD, 0);
  }
  else {
   mpitup = mpi_message(mpitup, 0, 1, MPI_COMM_WORLD, 0);
  }
 }
 MPI::Finalize();

 return 0;
}
