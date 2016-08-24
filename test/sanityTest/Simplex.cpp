#include "Simplex.h"


Simplex::Simplex(){
	SetOptimizationAlgorithmParameter("alpha",1);
	SetOptimizationAlgorithmParameter("beta",0.5);
	SetOptimizationAlgorithmParameter("gramma",1);
	SetOptimizationAlgorithmParameter("town",.5);
	SetOptimizationAlgorithmParameter("stepsize",0.5);	
}


Simplex::~Simplex(){}
Result Simplex::algorithm(Functiontobeoptimized* start){
	
	dimension=start->getparametersize();

	

	vertex A[dimension+1];
	cout<<"dimension ="<<dimension<<endl;
		
	//Initial

	for (auto it : start->parameters)
		A[0].par[it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
			if(std::distance(start->parameters.begin(),it)==i-1)
				A[i].par[it->getname()]=A[0].par[it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i].par[it->getname()]=A[0].par[it->getname()];
		}

	}

	//showfunc(start,A);

	for(int jj=0;jj<100;jj++){

	//Sort
		vertex M,Ar,Ac,Ae;
		for(int i=0;i<=dimension;i++){A[i].func=start->evaluate(A[i].par);}
		std::sort(A,A+dimension+1,[](vertex  & a, vertex   & b) -> bool{
		return a.func < b.func; });
	cout<<"jj= "<<jj<<endl;
	showfunc(A);

	//Mean
		for(int i=0;i<dimension;i++){
			for(auto it : start->parameters){
				M.par[it.getname()]+=A[i].par[it.getname()];		
			}
		}
		for (auto it : start->parameters){
			M.par[it.getname()]/=dimension;		
		}

	//calculate
		//Ar
		for (auto it : start->parameters){
			Ar.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M.par[it.getname()]-A[dimension].par[it.getname()]);		
		}
		Ar.func=start->evaluate(Ar.par);
		//Ae
		for (auto it : start->parameters){
			Ae.par[it.getname()]=Ar.par[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar.par[it.getname()]-M.par[it.getname()]);		
		}
		Ae.func=start->evaluate(Ae.par);
		//Ac
		if(Ar.func>A[dimension].func){
			for (auto it : start->parameters){
				Ac.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(A[dimension].par[it.getname()]-M.par[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar.par[it.getname()]-M.par[it.getname()]);			
			}
		}
		Ac.func=start->evaluate(Ac.par);
	//case//Update vertex step3
		//case 1
		if(Ar.func<A[0].func){
			if(Ae.func<A[0].func)
				for (auto it : start->parameters){A[dimension].par[it.getname()]=Ae.par[it.getname()];}
			else
				for (auto it : start->parameters){A[dimension].par[it.getname()]=Ar.par[it.getname()];}
		}	
		//case 2
		else if(Ar.func<A[dimension-1].func)
			for (auto it : start->parameters){A[dimension].par[it.getname()]=Ar.par[it.getname()];}
		//case 3
		else if(Ac.func<A[dimension].func)
			for (auto it : start->parameters){A[dimension].par[it.getname()]=Ac.par[it.getname()];}
		//case 4
		else{
			for(int i=1;i<=dimension;i++)
				for (auto it : start->parameters)	
					A[i].par[it.getname()]=GetOptimizationAlgorithmParameter("town")*A[0].par[it.getname()]+(1-GetOptimizationAlgorithmParameter("town"))*A[i].par[it.getname()];
		}

	//Return result
	}
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0].par[it.getname()];
		rs.result=A[0].func;
	return 	rs;
}

void Simplex::showfunc(vertex *para){
	
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].par.begin(); it!=para[i].par.end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<para[i].func<<endl;
	}
}
