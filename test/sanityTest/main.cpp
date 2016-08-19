#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "Simplex.h"
#include <utility>
int main(void){
      //Functiontobeoptimized objCal; 
	Myfunction test1;
	Himmelblaufunction test2;
	Boot_s_function test3;
	Michalewicz_function test4;
	Simplex altest;
      Minimizer mina;
      cout<<mina.minimize(&test1,&altest).result<<endl;

    return(0);
}
