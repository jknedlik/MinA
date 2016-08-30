#include "Simplex.h"


Simplex::Simplex(){
	setOptimizationAlgorithmParameter("alpha",1);
	setOptimizationAlgorithmParameter("beta",0.5);
	setOptimizationAlgorithmParameter("gramma",1);
	setOptimizationAlgorithmParameter("town",.5);
	setOptimizationAlgorithmParameter("stepsize",0.5);	
}


Simplex::~Simplex(){}
Result Simplex::algorithm(Functiontobeoptimized* start){
	
	dimension=start->getparametersize();
	vertexVector A;
	restore();
	//for(int i=0;i<1000;i++)std::cout<<i<<endl;
	if(Acopy.empty()){
		cout<<"dimension ="<<dimension<<endl;
       		//A=std::vector<vertex>();
		cout<<"dimension =3"<<dimension<<endl;
		A.resize(dimension+1);
		cout<<"dimension =22"<<dimension<<endl;

		
		//Initial

		for (auto it : start->parameters)
			A[0].first[it.getname()]=it.getstartingPoint();
	
	
		for(int i=1;i<=dimension;i++){
			for ( std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
				if(std::distance(start->parameters.begin(),it)==i-1)
					A[i].first[it->getname()]=A[0].first[it->getname()]+getOptimizationAlgorithmParameter("stepsize");
				else
					A[i].first[it->getname()]=A[0].first[it->getname()];
			}
	
		}
	}
	else {A=Acopy;

	}

	//showfunc(start,A);

	for(int jj=0;jj<100;jj++){
	
	//Sort
		vertex M,Ar,Ac,Ae;
		std::cout<<"hello world"<<std::endl;
		for(int i=0;i<=dimension;i++){A[i].second=start->evaluate(A[i].first);}
		std::cout<<"hello world2"<<std::endl;
		std::sort(A.begin(),A.end(),[](vertex  & a, vertex   & b) -> bool{
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
			Ar.first[it.getname()]=M.first[it.getname()]+getOptimizationAlgorithmParameter("alpha")*(M.first[it.getname()]-A[dimension].first[it.getname()]);		
		}
		Ar.second=start->evaluate(Ar.first);
		//Ae
		for (auto it : start->parameters){
			Ae.first[it.getname()]=Ar.first[it.getname()]+getOptimizationAlgorithmParameter("gramma")*(Ar.first[it.getname()]-M.first[it.getname()]);		
		}
		Ae.second=start->evaluate(Ae.first);
		//Ac
		if(Ar.second>A[dimension].second){
			for (auto it : start->parameters){
				Ac.first[it.getname()]=M.first[it.getname()]+getOptimizationAlgorithmParameter("beta")*(A[dimension].first[it.getname()]-M.first[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac.first[it.getname()]=M.first[it.getname()]+getOptimizationAlgorithmParameter("beta")*(Ar.first[it.getname()]-M.first[it.getname()]);			
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
					A[i].first[it.getname()]=getOptimizationAlgorithmParameter("town")*A[0].first[it.getname()]+(1-getOptimizationAlgorithmParameter("town"))*A[i].first[it.getname()];
		}
	Acopy=A;
	save();
	}
	//Return result*/
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0].first[it.getname()];
		rs.result=A[0].second;
	return 	rs;
}

void Simplex::showfunc(std::vector<vertex> &para){
	for(auto it:para){
          for (auto itx :it.first){std::cout << itx.first << "=" << itx.second << " ";}
	std::cout<<"\n";       
	 }
	
}
void Simplex::restore(){
	std::ifstream f(".Simplex.save");
	if(f.good()){
		Log::getLog() <<"Loading from file .Simplex.save";
		Log::getLog().flushLog();
		std::ifstream infile(".Simplex.save");
   	 	boost::archive::text_iarchive ia(infile);
		std::map <string, double> additionalInformation;
		
    		ia >>Acopy;
		ia >>additionalInformation;
		setAdditionalInformation(additionalInformation);
	}
  }
void Simplex::save()const{
	if(!Acopy.empty()){	
	std::ofstream outfile(".Simplex.save");
	boost::archive::text_oarchive oa(outfile);
	oa<<Acopy;
	auto ai = getAdditionalInformation();
	 oa<<ai;
	}
}
