#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "Simplex.h"
int main(void){
      //Functiontobeoptimized objCal; 
      Myfunction test;
      Simplex altest;
      Minimizer mina;
	std::map <string, double> grade_list;
	grade_list["x"] =1.1;
	grade_list["y"] =0.52;
	cout<<mina.minimize(&test,&altest).result<<endl;
	cout<<test.evaluate(grade_list);
    return(0);
}
