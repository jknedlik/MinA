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
		A[0].first[it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
			if(std::distance(start->parameters.begin(),it)==i-1)
				A[i].first[it->getname()]=A[0].first[it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i].first[it->getname()]=A[0].first[it->getname()];
		}

	}

	//showfunc(start,A);

	for(int jj=0;jj<100;jj++){

	//Sort
		vertex M,Ar,Ac,Ae;
		for(int i=0;i<=dimension;i++){A[i].second=start->evaluate(A[i].first);}
		std::sort(A,A+dimension+1,[](vertex  & a, vertex   & b) -> bool{
		return a.second < b.second; });
	cout<<"jj= "<<jj<<endl;
	showfunc(A);

	//Mean
		for(int i=0;i<dimension;i++){
			for(auto it : start->parameters){
				M.first[it.getname()]+=A[i].first[it.getname()];		
			}
		}
		for (auto it : start->parameters){
			M.first[it.getname()]/=dimension;		
		}

	//calculate
		//Ar
		for (auto it : start->parameters){
			Ar.first[it.getname()]=M.first[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M.first[it.getname()]-A[dimension].first[it.getname()]);		
		}
		Ar.second=start->evaluate(Ar.first);
		//Ae
		for (auto it : start->parameters){
			Ae.first[it.getname()]=Ar.first[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar.first[it.getname()]-M.first[it.getname()]);		
		}
		Ae.second=start->evaluate(Ae.first);
		//Ac
		if(Ar.second>A[dimension].second){
			for (auto it : start->parameters){
				Ac.first[it.getname()]=M.first[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(A[dimension].first[it.getname()]-M.first[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac.first[it.getname()]=M.first[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar.first[it.getname()]-M.first[it.getname()]);			
			}
		}
		Ac.second=start->evaluate(Ac.first);
	//case//Update vertex step3
		//case 1
		if(Ar.second<A[0].second){
			if(Ae.second<A[0].second)
				for (auto it : start->parameters){A[dimension].first[it.getname()]=Ae.first[it.getname()];}
			else
				for (auto it : start->parameters){A[dimension].first[it.getname()]=Ar.first[it.getname()];}
		}	
		//case 2
		else if(Ar.second<A[dimension-1].second)
			for (auto it : start->parameters){A[dimension].first[it.getname()]=Ar.first[it.getname()];}
		//case 3
		else if(Ac.second<A[dimension].second)
			for (auto it : start->parameters){A[dimension].first[it.getname()]=Ac.first[it.getname()];}
		//case 4
		else{
			for(int i=1;i<=dimension;i++)
				for (auto it : start->parameters)	
					A[i].first[it.getname()]=GetOptimizationAlgorithmParameter("town")*A[0].first[it.getname()]+(1-GetOptimizationAlgorithmParameter("town"))*A[i].first[it.getname()];
		}

	//Return result
	}
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0].first[it.getname()];
		rs.result=A[0].second;
	return 	rs;
}

void Simplex::showfunc(vertex *para){
	
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].first.begin(); it!=para[i].first.end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<para[i].second<<endl;
	}
}
