#include "MinA/algorithm/MetaT.h"
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/TestFunctions.hpp"
#include "MinA/common/algorithm.hpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <utility>
using namespace std;

int main(int argc, char** argv)
{
 //  std::make_tuple(std::index_sequence<5>...);
 //  for_each_tuple(s, [](size_t a) { std::cout << a << std::endl; });
 auto y = std::make_tuple(3, 3, 2);
 auto x = std::make_tuple(6, 4, 4);

 //  std::cout << a << std::endl;
 for_each_tuple(y, x, [](auto& r, auto& p) {
  r = 1;
  p = 5;
 });
 for_each_tuple_i(y, x, [](size_t index, auto& r, auto& p) {
  std::cout << index << " =" << r << "|" << p << std::endl;
 });
 return (0);
}
