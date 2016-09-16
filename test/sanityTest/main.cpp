#include <iostream>
#include <map>
#include "mpi.h"
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "TestFunction.cpp"
#include "MinA/algorithm/Simplex.h"
#include "MinA/algorithm/SimplexParallel.h"
#include <utility>

int main(int argc, char** argv)
{

    if (!MPI::Is_initialized())
        MPI::Init();
    Minimizer mina;
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    std::shared_ptr<Simplex> altest1(new Simplex(100));
    altest1->setFunctionName("Square");
    std::shared_ptr<SimplexParallel> altest2(new SimplexParallel(1000));
    altest2->setFunctionName("Square");
    altest2->setOptimizationAlgorithmParameter("beta", 0.2);
    altest2->setOptimizationAlgorithmParameter("tau", 0.2);

    std::shared_ptr<SquareFunction> test5(new SquareFunction(10));
    std::shared_ptr<McCormick_function> test31(new McCormick_function);
    std::shared_ptr<Modify_Matthias_function> test32(new Modify_Matthias_function(6));
    std::shared_ptr<Gaussian_function> gauss(new Gaussian_function(10));
    
    std::shared_ptr<Schwefel_function> test1(new Schwefel_function(1));
    /*
    map<string, double> input;
    double s,e,r;
    s=-5;
    e=5;
    r=0.05;
  
   	 ofstream data;
   	 data.open("z_test_function_plot", std::ios::app);
	 for(double i=s;i<=e;i+=0.01){
		input["x0"]=i;
		data<<" "<<i<<" "<<test5->getEvaluation(input)<<std::endl;
	
	}
    data.close();*/
    
    vector<double> stepSize;
    stepSize.resize(test5->getParameterSize());

/*    for (int i = 0; i < test5->getParameterSize(); i++)
    stepSize[i] = 1;
    altest1->setStepSize(stepSize);
    altest2->setStepSize(stepSize);*/
    if (world_rank == 0)
        cout << "function minimized value=" << mina.minimize(gauss, altest2).result << endl;
    else
        auto rs = mina.minimize(gauss, altest2);
    MPI_Finalize();
    return (0);
}
