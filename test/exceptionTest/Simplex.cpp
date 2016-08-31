#include "Simplex.h"


Simplex::Simplex(){
	setOptimizationAlgorithmParameter("alpha",1);
	setOptimizationAlgorithmParameter("beta",0.5);
	setOptimizationAlgorithmParameter("gramma",1);
	setOptimizationAlgorithmParameter("town",.5);
	setOptimizationAlgorithmParameter("stepsize",0.5);	
	setAdditionalInformation("stopingiteration","20");
	setAdditionalInformation("currentiteration","0");
}


Simplex::~Simplex(){}
Result Simplex::algorithm(Functiontobeoptimized* start){
	
	function=start;
	dimension=start->getparametersize();
	vertexVector A;
	restore();
	if(Acopy.empty()){
		
		A.resize(dimension+1);
		
		//Initial

		InitialVertex(A);
	}
	else {A=Acopy;

	}

	//showfunc(start,A);

	while(checkStopingCondition()){
	

		vertex Ar,Ac,Ae;
		for(int i=0;i<=dimension;i++){A[i].second=start->evaluate(A[i].first);}
		
		std::sort(A.begin(),A.end(),[](vertex  & a, vertex   & b) -> bool{return a.second < b.second; });//Sort
		cout<<"loop= "<<getAdditionalInformation()["currentiteration"]<<endl;
		showVertex(A);
		vertex M;
		calculateM(A,M,2);//Mean
	
		int check=0;
		calculateAr(Ar,M,A[dimension]);//Ar
				if(Ar.second<A[0].second){
					calculateAe(Ae,M,Ar);//Ae
					if(Ae.second<A[0].second){push(A[dimension],Ae);check=1;}
					else{push(A[dimension],Ar);check=2;}
				}//case 1	
				else if(Ar.second<A[dimension-1].second){push(A[dimension],Ar);check=3;}//case 2 
				else {
					if(Ar.second>A[dimension].second){
						calculateAc(Ac,M,A[dimension]);
					}else{
					calculateAc(Ac,M,Ar);
					}//Ac
					if(Ac.second<A[dimension].second){		
						push(A[dimension],Ac);
						check=4;
					}
	     			}//case 3
				if(check==0)newVertex(A);
		Acopy=A;
		save();		
	}
	//Return result*/
	Result rs;
	pushResult(rs,A[0]);//Result
	return 	rs;
}

bool Simplex::checkStopingCondition(){
	if(stoi(getAdditionalInformation()["currentiteration"])>=stoi(getAdditionalInformation()["stopingiteration"]))return false;
		setAdditionalInformation("currentiteration",std::to_string(stoi(getAdditionalInformation()["currentiteration"])+1));
	return true;
}

void Simplex::showVertex(vertexVector &para){
	int ii=0;
	for(auto it:para){
          for (auto itx :it.first){std::cout << itx.first << "=" << itx.second << " ";}
	  cout<<"f(A"<<ii<<")="<<it.second<<"\n";
	ii++;        
	}
}

void Simplex::showVertex(vertex &para){
	
	for (std::map<string, double>::iterator it=para.first.begin(); it!=para.first.end(); ++it)
    		std::cout << it->first << "=" << it->second << " ";	
	cout<<"f()="<<para.second<<endl;
	
}
void Simplex::push(vertex &a,vertex &b){
	for (auto it : function->parameters)a.first[it.getname()]=b.first[it.getname()];
	a.second=b.second;
}

void Simplex::pushResult(Result &rs,vertex &A){
	for (auto it : function->parameters)
		rs.optimizationparameter["it.getname()"]=A.first[it.getname()];
		rs.result=A.second;	
}

void Simplex::InitialVertex(vertexVector &A){
	for (auto it : function->parameters)
		A[0].first[it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=function->parameters.begin(); it!=function->parameters.end(); ++it){
			if(std::distance(function->parameters.begin(),it)==i-1)
				A[i].first[it->getname()]=A[0].first[it->getname()]+getOptimizationAlgorithmParameter("stepsize");
			else
				A[i].first[it->getname()]=A[0].first[it->getname()];
		}

	}
}

void Simplex::calculateM(vertexVector &A,vertex &M,int world_size){
	for(int i=0;i<= dimension -world_size+1;i++){
		for(auto it : function->parameters){
			M.first[it.getname()]+=A[i].first[it.getname()];		
		}
	}
	for (auto it : function->parameters){
		M.first[it.getname()]/=(dimension-world_size+2);		
	}
}
void Simplex::calculateAr(vertex &Ar,vertex &M,vertex &Aj){
	for (auto it : function->parameters)
		Ar.first[it.getname()]=M.first[it.getname()]+getOptimizationAlgorithmParameter("alpha")*(M.first[it.getname()]-Aj.first[it.getname()]);		
	Ar.second=function->evaluate(Ar.first);
}

void Simplex::calculateAe(vertex &Ae,vertex &M,vertex &Ar){
	for (auto it : function->parameters)
		Ae.first[it.getname()]=Ar.first[it.getname()]+getOptimizationAlgorithmParameter("gramma")*(Ar.first[it.getname()]-M.first[it.getname()]);	
	Ae.second=function->evaluate(Ae.first);			
}

void Simplex::calculateAc(vertex &Ac,vertex &M,vertex &Ajp){
	for (auto it : function->parameters)
	Ac.first[it.getname()]=M.first[it.getname()]+getOptimizationAlgorithmParameter("beta")*(Ajp.first[it.getname()]-M.first[it.getname()]);			
	Ac.second=function->evaluate(Ac.first);	
}


void Simplex::newVertex(vertexVector &A){
	for(int i=1;i<=dimension;i++)
		for (auto it : function->parameters) 	
			A[i].first[it.getname()]=getOptimizationAlgorithmParameter("tau")*A[0].first[it.getname()]+(1-getOptimizationAlgorithmParameter("tau"))*A[i].first[it.getname()];
	
}

void Simplex::restore(){
	std::ifstream f(".Simplex.save");
	if(f.good()){
		Log::getLog() <<"Loading from file .Simplex.save";
		Log::getLog().flushLog();
		std::ifstream infile(".Simplex.save");
   	 	boost::archive::text_iarchive ia(infile);
		std::map <string, double> additionalInformation;
		
    		//ia >>Acopy;
		//ia >>additionalInformation;
		//setAdditionalInformation(additionalInformation);
	}
  }
void Simplex::save()const{
	if(!Acopy.empty()){	
	std::ofstream outfile(".Simplex.save");
	boost::archive::text_oarchive oa(outfile);
	oa<<Acopy;
//	auto ai = getAdditionalInformation();
//	 oa<<ai;
	}
}
