#include <iostream>
#include <map>
#include "MinA/common/Minimizer.h"
#include "MinA/common/Result.h"
#include "myfunction.cpp"
#include "Simplex.h"
#include <utility>
int main(void){
      //Functiontobeoptimized objCal; 
      Myfunction test;
      Simplex altest;
      Minimizer mina;
      cout<<mina.minimize(&test,&altest).result<<endl;

    return(0);
}
