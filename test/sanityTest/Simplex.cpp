#include "Simplex.h"
#include <algorithm>
Simplex::Simplex(){
	SetOptimizationAlgorithmParameter("alpha",1);
	SetOptimizationAlgorithmParameter("beta",0.5);
	SetOptimizationAlgorithmParameter("gramma",1);
	SetOptimizationAlgorithmParameter("town",0.5);
	SetOptimizationAlgorithmParameter("stepsize",0.5);	
}
Simplex::~Simplex(){}
Result Simplex::algorithm(Functiontobeoptimized* start){
	int dimension=start->getparametersize();
	//double A[dimension+1][dimension];
	double func[dimension+1];	
	//double alpha=1,beta=0.5,gramma=1,town=0.5;
//dimension=2;	
std::map <string, double> A[dimension+1];
cout<<dimension<<" ";

	
//Initial
A[0]["x"]=1;A[0]["y"]=3;
    	A[1]["x"]=-4;A[1]["y"]=-4;
    	A[2]["x"]=1;A[2]["y"]=-8;
/*
int d=9;
for(int i=0;i<=dimension;i++){

	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	A[i][it->getname()]=d;
	d--;
	}

}*/

showfunc(start,A);
for(int jj=0;jj<100;jj++){
//cout<<"//////////////////="<<jj<<endl;
//Sort
std::map <string, double> M,Ar,Ac,Ae;
std::sort(A,A+dimension+1,[start](std::map <string, double>  & a, std::map <string, double>  & b) -> bool{
//Myfunction startt;
return start->evaluate(a) < start->evaluate(b) ; });
showfunc(start,A);

//Mean
for(int i=0;i<dimension;i++)
{
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	M[it->getname()]+=A[i][it->getname()];		
	}
}
for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	M[it->getname()]/=dimension;		
	}
for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	cout<<it->getname()<<"->"<<M[it->getname()];		
	}
//calculate
	//Ar
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ar[it->getname()]=M[it->getname()]+GetOptimizationAlgorithmParameter("alpha")*(M[it->getname()]-A[dimension][it->getname()]);		
	}
	//Ae
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ae[it->getname()]=Ar[it->getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar[it->getname()]-M[it->getname()]);		
	}
	//Ac
	if(start->evaluate(Ar)>start->evaluate(A[dimension])){
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ac[it->getname()]=M[it->getname()]+GetOptimizationAlgorithmParameter("beta")*(A[dimension][it->getname()]-M[it->getname()]);			
	}
	}
	else{
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ac[it->getname()]=M[it->getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar[it->getname()]-M[it->getname()]);			
	}
	}
//case

//Update vertex step3
	//case 1
	if(start->evaluate(Ar)<start->evaluate(A[0])){
	
		if(start->evaluate(Ae)<start->evaluate(A[0]))
		for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][it->getname()]=Ae[it->getname()];}
		else
		for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][it->getname()]=Ar[it->getname()];}
	}
	//case 2
	else if(start->evaluate(Ar)<start->evaluate(A[dimension-1])){
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][it->getname()]=Ar[it->getname()];}
	}
	//case 3
	else if(start->evaluate(Ac)<start->evaluate(A[dimension]))
	for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][it->getname()]=Ac[it->getname()];}
	//case 4
	else{
		for(int i=1;i<=dimension;i++){
		for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){	
			A[i][it->getname()]=GetOptimizationAlgorithmParameter("town")*A[0][it->getname()]+(1-GetOptimizationAlgorithmParameter("town"))*A[i][it->getname()];
			
		}	
		}
	}
///////////////


	
}
Result rs;
for (std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it)
rs.optimizationparameter["it->getname()"]=A[0][it->getname()];
rs.result=start->evaluate(A[0]);
//*/

return 	rs;
}

void Simplex::showfunc(Functiontobeoptimized* start,std::map <string, double> *para){
	int dimension=start->getparametersize();
	for(int i=0;i<=dimension;i++){
	for (std::map<string, double>::iterator it=para[i].begin(); it!=para[i].end(); ++it)
    		std::cout << it->first << "=" << it->second << " ";	
	cout<<"f(A"<<i<<")="<<start->evaluate(para[i])<<endl;
	}
}
