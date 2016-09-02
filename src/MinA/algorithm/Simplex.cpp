#include "MinA/algorithm/Simplex.h"


Simplex::Simplex(int stop){
	stoppingIteration=stop;
	setOptimizationAlgorithmParameter("alpha",1);
	setOptimizationAlgorithmParameter("beta",0.5);
	setOptimizationAlgorithmParameter("gramma",1);
	setOptimizationAlgorithmParameter("town",.5);	
	setAdditionalInformation("stopingiteration","100");
	setAdditionalInformation("checkboundary","");
	setAdditionalInformation("stopingConditionType","iteration");
	setAdditionalInformation("stopingdifference","0.01");
	currentiteration=0;	
	difference=100;
	
}


Simplex::~Simplex(){}
Result Simplex::algorithm(std::shared_ptr<Functiontobeoptimized> start){
	
	function=start;
	dimension=start->getParameterSize();
	setStepSize();

	vertexVector A;
	restore();
	if(Acopy.empty()){
		
		A.resize(dimension+1);
		
		//Initial

		createInitialVertex(A);
	}
	else {A=Acopy;
	}	
	cout<<"Start with current loop= "<<currentiteration<<endl;
	for(int i=0;i<=dimension;i++)
	checkBoundaryCondition(A[i]);
	while(checkStopingCondition()){
	
		
		vertex Ar,Ac,Ae;
		for(int i=0;i<=dimension;i++){A[i].second=start->evaluate(A[i].first);}
		
		std::sort(A.begin(),A.end(),[](vertex  & a, vertex   & b) -> bool{return a.second < b.second; });//Sort
		cout<<"loop= "<<currentiteration<<endl;
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
				if(check==0)createNewVertex(A);
		//if(currentiteration==39)for (auto it : function->parameters) A[dimension].first[it.getName()]=1000;

		checkBoundaryCondition(A[dimension]);
		Acopy=A;
		save();		

		ofstream dataFile;
		dataFile.open("dataFile.txt",std::ios::app);
		dataFile<<"Iteration= "<<currentiteration<<"	f(A0)= "<<A[0].second<<"	";
		for (std::map<string, double>::iterator it=A[0].first.begin(); it!=A[0].first.end(); ++it)
			dataFile<<it->first<< " = "<<it->second<<"	";
		dataFile<<"\n";
		dataFile.close();
		difference=(A[0].second-A[1].second)*(A[0].second-A[1].second);
	

	}
	//Return result*/
	Result rs;
	pushResult(rs,A[0]);
	return 	rs;
}
void Simplex::setStepSize(){
	if(stepSize.empty()){
	stepSize.resize(dimension);
	for(int i=0;i<dimension;i++)
	stepSize[i]=0.5;	
	}

}
void Simplex::setStepSize(vector<double> s){
	
	stepSize=s;
}

bool Simplex::checkStopingCondition(){

	if(getAdditionalInformation()["stopingConditionType"]=="iteration"){
		if(currentiteration>=stoppingIteration){			
			return false;
		}
	}
	else if(getAdditionalInformation()["stopingConditionType"]=="difference"){
		if(stod(getAdditionalInformation()["stopingdifference"])>=difference)
			return false;
	}	

	if(getAdditionalInformation()["checkboundary"]!=""){
		std::cout<<getAdditionalInformation()["checkboundary"]<<"\n";			
		return false;
	}
	if(currentiteration>5000){
		cout<<"Number of iteration is maximum";
		return false;
	}
		currentiteration++;
	return true;
}
void Simplex::checkBoundaryCondition(vertex &A){
	int check=0;
	
	for (auto it : function->parameters){
		if(it.getBoundaryLeft()!=nullptr)		
			if(A.first[it.getName()]<*it.getBoundaryLeft())check++;	

		if(it.getBoundaryRight()!=nullptr)		
			if(A.first[it.getName()]>*it.getBoundaryRight())check++;
	}
	if(check!=0){setAdditionalInformation("checkboundary","out of boundary");}
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
	for (auto it : function->parameters)a.first[it.getName()]=b.first[it.getName()];
	a.second=b.second;
}

void Simplex::pushResult(Result &rs,vertex &A){
	for (auto it : function->parameters)
		rs.optimizationParameter["it.getName()"]=A.first[it.getName()];
		rs.result=A.second;	
}

void Simplex::createInitialVertex(vertexVector &A){
	for (auto it : function->parameters)
		A[0].first[it.getName()]=*(it.getStartingPoint());

	
	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=function->parameters.begin(); it!=function->parameters.end(); ++it){
			if(std::distance(function->parameters.begin(),it)==i-1)
				A[i].first[it->getName()]=A[0].first[it->getName()]+stepSize[i-1];
			else
				A[i].first[it->getName()]=A[0].first[it->getName()];
		}

	}
}

void Simplex::calculateM(vertexVector &A,vertex &M,int world_size){
	for(int i=0;i<= dimension -world_size+1;i++){
		for(auto it : function->parameters){
			M.first[it.getName()]+=A[i].first[it.getName()];		
		}
	}
	for (auto it : function->parameters){
		M.first[it.getName()]/=(dimension-world_size+2);		
	}
}
void Simplex::calculateAr(vertex &Ar,vertex &M,vertex &Aj){
	for (auto it : function->parameters)
		Ar.first[it.getName()]=M.first[it.getName()]+getOptimizationAlgorithmParameter("alpha")*(M.first[it.getName()]-Aj.first[it.getName()]);		
	Ar.second=function->evaluate(Ar.first);
}

void Simplex::calculateAe(vertex &Ae,vertex &M,vertex &Ar){
	for (auto it : function->parameters)
		Ae.first[it.getName()]=Ar.first[it.getName()]+getOptimizationAlgorithmParameter("gramma")*(Ar.first[it.getName()]-M.first[it.getName()]);	
	Ae.second=function->evaluate(Ae.first);			
}

void Simplex::calculateAc(vertex &Ac,vertex &M,vertex &Ajp){
	for (auto it : function->parameters)
	Ac.first[it.getName()]=M.first[it.getName()]+getOptimizationAlgorithmParameter("beta")*(Ajp.first[it.getName()]-M.first[it.getName()]);			
	Ac.second=function->evaluate(Ac.first);	
}


void Simplex::createNewVertex(vertexVector &A){
	for(int i=1;i<=dimension;i++)
		for (auto it : function->parameters) 	
			A[i].first[it.getName()]=getOptimizationAlgorithmParameter("tau")*A[0].first[it.getName()]+(1-getOptimizationAlgorithmParameter("tau"))*A[i].first[it.getName()];
	
}

void Simplex::restore(){
	std::ifstream f(".Simplex.save");
	if(f.good()){
		Log::getLog() <<"Loading from file .Simplex.save\n";
		Log::getLog().flushLog();
		std::ifstream infile(".Simplex.save");
   	 	boost::archive::text_iarchive ia(infile);
    		ia >>Acopy;
		ia>>currentiteration;
	}
  }
void Simplex::save()const{
	if(!Acopy.empty()){	
		std::ofstream outfile(".Simplex.save");
		boost::archive::text_oarchive oa(outfile);
		oa<<Acopy;
		oa<<currentiteration;
	}
}
