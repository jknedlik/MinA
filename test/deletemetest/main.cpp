
#include <iostream>
#include <map>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
using namespace std;
typedef int A;
typedef int B;
typedef double C;
template <typename... Types>
struct foo {
};
template <typename A template <typename... T>, typename B template <... X>>
static_assert(std::is_base_of<std::tuple<>, X>>, "");
std::static_assert(std::is_base_of<std::tuple<>, Y>>, "");
struct tt {
    X x Y y;
};
;
int main(int argc, char** argv) { std::tuple<A, B, C> t{ 0, 1, 1.0 }; }
