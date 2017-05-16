#include "MinA/algorithm/MetaOptimizer.h"
#include "MinA/algorithm/Simplex.h"
#include "MinA/algorithm/SimplexParallel.h"
#include "MinA/algorithm/SimplexT.h"

#include "MinA/common/Minimizer.h"
#include "MinA/common/MinimizerT.h"
#include "MinA/common/Result.h"
#include "TestFunction.cpp"
#include "mpi.h"
#include <iostream>
#include <map>
#include <memory>
#include <utility>
using namespace std;

int main(int argc, char** argv)
{
    // Call Simplex with new Syntax
    MinimizerT<Simplex, McCormick_function> mina(
      Simplex(), McCormick_function({ "x", "y" }, { 0., 0. }, { -1.5, 3.0 }, { 4., 4. }));
    cout << mina().result << endl; // calls mina.minimize()
                                   // call ParallelSimplex with new Syntax
    MPI::Init();
    MinimizerT<SimplexParallel, McCormick_function> minaP(
      SimplexParallel(), McCormick_function({ "x", "y" }, { 0., 0. }, { -1.5, 3.0 }, { 4., 4. }));
    cout << minaP().result << endl; // calls mina.minimize(
    MPI_Finalize();
    std::cout << "dsgsfddsfgsdfsdg" << std::endl;
    MetaOptimizer<SimplexT, McCormick_function, 1> meta(std::array<std::size_t, 1>{ 1 });
    auto x = // McCormick_function({ "x", "y" }, { 0., 0. }, { -1.5, 3.0 }, { 4., 4. });
      std::make_shared<McCormick_function>(
        McCormick_function({ "x", "y" }, { 0., 0. }, { -1.5, 3.0 }, { 4., 4. }));
    auto r = meta.algorithm(x);
    return (0);
}
