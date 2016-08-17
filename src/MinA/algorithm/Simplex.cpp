#include "MinA/algorithm/Simplex.h"
#include <algorithm>

Result Simplex::algorithm(Functiontobeoptimized* start){
	int dimension=start->getparametersize();
	//double A[dimension+1][dimension];
	double func[dimension+1];	
	//double alpha=1,beta=0.5,gramma=1,town=0.5;
	std::map <string, double> A[dimension+1];
	
//Initial
A[0]["x"]=1;A[0]["y"]=3;
    	A[1]["x"]=-4;A[1]["y"]=-4;
    	A[2]["x"]=1;A[2]["y"]=-8;
/*int d=9;
for(int i=0;i<=dimension;i++){

	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	A[i][*it]=d;
	d--;
	}

}*/
showfunc(start,A);
for(int jj=0;jj<100;jj++){
//cout<<"//////////////////="<<jj<<endl;
//Sort
std::map <string, double> M,Ar,Ac,Ae;
std::sort(A,A+dimension+1,[](std::map <string, double>  & a, std::map <string, double>  & b) -> bool{
return start->evaluate(a) < start->evaluate(b) ; });
showfunc(start,A);
//Mean
for(int i=0;i<dimension;i++)
{
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	M[*it]+=A[i][*it];		
	}
}
for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	M[*it]/=dimension;		
	}
/*for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	cout<<*it<<"->"<<M[*it];		
	}*/
//calculate
	//Ar
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ar[*it]=M[*it]+GetOptimizationAlgorithmParameter("alpha")*(M[*it]-A[dimension][*it]);		
	}
	//Ae
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ae[*it]=Ar[*it]+GetOptimizationAlgorithmParameter("gramma")*(Ar[*it]-M[*it]);		
	}
	//Ac
	if(start->evaluate(Ar)>start->evaluate(A[dimension])){
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ac[*it]=M[*it]+GetOptimizationAlgorithmParameter("beta")*(A[dimension][*it]-M[*it]);			
	}
	}
	else{
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
	Ac[*it]=M[*it]+GetOptimizationAlgorithmParameter("beta")*(Ar[*it]-M[*it]);			
	}
	}
//case

//Update vertex step3
	//case 1
	if(start->evaluate(Ar)<start->evaluate(A[0])){
	
		if(start->evaluate(Ae)<start->evaluate(A[0]))
		for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][*it]=Ae[*it];}
		else
		for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][*it]=Ar[*it];}
	}
	//case 2
	else if(start->evaluate(Ar)<start->evaluate(A[dimension-1])){
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][*it]=Ar[*it];}
	}
	//case 3
	else if(start->evaluate(Ac)<start->evaluate(A[dimension]))
	for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){A[dimension][*it]=Ac[*it];}
	//case 4
	else{
		for(int i=1;i<=dimension;i++){
		for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){	
			A[i][*it]=GetOptimizationAlgorithmParameter("town")*A[0][*it]+(1-GetOptimizationAlgorithmParameter("town"))*A[i][*it];
			
		}	
		}
	}
///////////////


	
}
Result rs;
for (std::set<string>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it)
rs.optimizationparameter["*it"]=A[0][*it];
rs.result=start->evaluate(A[0]);

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
