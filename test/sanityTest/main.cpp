#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "Simplex.h"
#include <utility>

int main(void){
      
	Myfunction test1;
	Himmelblaufunction test2;
	Boot_s_function test3;
	Michalewicz_function test4;
	Simplex altest;
	Minimizer mina;
        cout<<"function minimized value="<<mina.minimize(&test2,&altest).result<<endl;

    return(0);
}
