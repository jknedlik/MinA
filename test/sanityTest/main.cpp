#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "TestFunction.cpp"
#include "Simplex.h"
#include "SimplexParallel.h"
#include <utility>

int main(void){
      
	Myfunction test1;
	Himmelblaufunction test2;
	Boot_s_function test3;
	Michalewicz_function test4;
	Matthias_function test5;
	Simplex altest1;
	SimplexParallel altest2;
	Minimizer mina;
	altest1.setAdditionalInformation("stopingiteration","200");
	altest2.setAdditionalInformation("stopingiteration","100");
        cout<<"function minimized value="<<mina.minimize(&test5,&altest1).result<<endl;

    return(0);
}
