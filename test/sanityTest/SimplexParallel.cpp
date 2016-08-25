#include "SimplexParallel.h"


SimplexParallel::SimplexParallel(){
	SetOptimizationAlgorithmParameter("alpha",1);
	SetOptimizationAlgorithmParameter("beta",0.5);
	SetOptimizationAlgorithmParameter("gramma",1);
	SetOptimizationAlgorithmParameter("tau",.5);
	SetOptimizationAlgorithmParameter("stepsize",0.5);	
}


SimplexParallel::~SimplexParallel(){}
Result SimplexParallel::algorithm(Functiontobeoptimized* start){
	
	function=start;
	dimension=function->getparametersize();
	vertex A[dimension+1];
	InitialVertex(A);//Initial
	
	MPI_Init(NULL, NULL);
  	int world_rank;
  	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        
	for(int jj=0;jj<100;jj++){

	if(world_rank==0){
	

		for(int i=0;i<=dimension;i++){A[i].func=function->evaluate(A[i].par);}//Evaluate(A)

		std::sort(A,A+dimension+1,[](vertex  & a, vertex   & b) -> bool{return a.func < b.func; });//Sort
	
		cout<<"loop= "<<jj<<endl;
		showVertex(A);
	
		vertex M;
		calculateM(A,M,world_size);//Mean
		
		for(int i=1;i<world_size;i++){
		sendVertex(M,i,0);
		sendVertex(A[0],i,1);
		sendVertex(A[dimension-i],i,2);
		sendVertex(A[dimension-i+1],i,3);
		}//sending M,A0,Aj_1,Aj	
	}

        if (world_rank !=0){
	
	vertex M,Aj,Aj_1,A0,Ar,Ac,Ae;
	M=receiveVertex(0,0);
	A0=receiveVertex(0,1);	
	Aj_1=receiveVertex(0,2);	
	Aj=receiveVertex(0,3);//receiving M,A0,Aj,Aj_1
	
	//calculate
		A0.func=function->evaluate(A0.par);
		Aj_1.func=function->evaluate(Aj_1.par);
		Aj.func=function->evaluate(Aj.par);
		int check=0;
		
	//Update vertex step3
		calculateAr(Ar,M,Aj);//Ar
		if(Ar.func<A0.func){
			calculateAe(Ae,M,Ar);//Ae
			if(Ae.func<A0.func){push(Aj,Ae);check=1;}
			else{push(Aj,Ar);check=2;}
		}//case 1	
		else if(Ar.func<Aj_1.func){push(Aj,Ar);check=3;}//case 2 
		else {
			if(Ar.func>Aj.func){
				calculateAc(Ac,M,Aj);
			}else{
				calculateAc(Ac,M,Ar);
			}//Ac
			if(Ac.func<Aj.func){		
				push(Aj,Ac);
				check=4;
			}
		}//case 3
		
			sendVertex(Aj,0,world_rank);
			MPI_Send(&(check), 1, MPI_INT, 0, world_rank+1, MPI_COMM_WORLD);		
		}

	if(world_rank==0){
		
		int sumcheck=0;
		for(int i=1;i<world_size;i++){
			int check;
			MPI_Recv(&(check), 1, MPI_INT, i, i+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			sumcheck+=check;
			vertex b=receiveVertex(i,i);
			push(A[dimension-i+1],b);
		}
			
		if(sumcheck==0){
			newVertex(A);
		}	//case 4
		
	}
}
	
		
	if(world_rank==0){		
	
	Result rs;
	pushResult(rs,A[0]);//Result
		
	return 	rs;
	}
	MPI_Finalize();
	 
}

void SimplexParallel::showVertex(vertex *para){
	
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].par.begin(); it!=para[i].par.end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<para[i].func<<endl;
	}
}

void SimplexParallel::showM(vertex &para){
	
	for (std::map<string, double>::iterator it=para.par.begin(); it!=para.par.end(); ++it)
    		std::cout << it->first << "=" << it->second << " ";	
	cout<<"f(M)="<<para.func<<endl;
	para.func=100;
}


void SimplexParallel::calculateM(vertex *A,vertex &M,int world_size){
	for(int i=0;i<= dimension -world_size+1;i++){
		for(auto it : function->parameters){
			M.par[it.getname()]+=A[i].par[it.getname()];		
		}
	}
	for (auto it : function->parameters){
		M.par[it.getname()]/=(dimension-world_size+2);		
	}
}

void SimplexParallel::calculateAr(vertex &Ar,vertex &M,vertex &Aj){
	for (auto it : function->parameters)
		Ar.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M.par[it.getname()]-Aj.par[it.getname()]);		
	Ar.func=function->evaluate(Ar.par);
}

void SimplexParallel::calculateAe(vertex &Ae,vertex &M,vertex &Ar){
	for (auto it : function->parameters)
		Ae.par[it.getname()]=Ar.par[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar.par[it.getname()]-M.par[it.getname()]);	
	Ae.func=function->evaluate(Ae.par);			
}

void SimplexParallel::calculateAc(vertex &Ac,vertex &M,vertex &Ajp){
	for (auto it : function->parameters)
	Ac.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ajp.par[it.getname()]-M.par[it.getname()]);			
	Ac.func=function->evaluate(Ac.par);	
}

void SimplexParallel::push(vertex &a,vertex &b){
	for (auto it : function->parameters)a.par[it.getname()]=b.par[it.getname()];
	a.func=b.func;
}

void SimplexParallel::pushResult(Result &rs,vertex &A){
	for (auto it : function->parameters)
		rs.optimizationparameter["it.getname()"]=A.par[it.getname()];
		rs.result=A.func;	
}

void SimplexParallel::newVertex(vertex *A){
	for(int i=1;i<=dimension;i++)
		for (auto it : function->parameters)	
			A[i].par[it.getname()]=GetOptimizationAlgorithmParameter("tau")*A[0].par[it.getname()]+(1-GetOptimizationAlgorithmParameter("tau"))*A[i].par[it.getname()];

}

void SimplexParallel::InitialVertex(vertex *A){
	for (auto it : function->parameters)
		A[0].par[it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=function->parameters.begin(); it!=function->parameters.end(); ++it){
			if(std::distance(function->parameters.begin(),it)==i-1)
				A[i].par[it->getname()]=A[0].par[it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i].par[it->getname()]=A[0].par[it->getname()];
		}

	}
}
void SimplexParallel::sendVertex(vertex &An,int receiver,int tag){
			double Ad[dimension];	
			int run=0;
			for (auto it : function->parameters){
			Ad[run]=An.par[it.getname()];		
			run++;
			}

			MPI_Send(&(Ad[0]), dimension, MPI_DOUBLE, receiver, tag, MPI_COMM_WORLD);
	
}
vertex SimplexParallel::receiveVertex(int sender,int tag){
			vertex An;			
			double Ad[dimension];
			MPI_Recv(&(Ad[0]), dimension, MPI_DOUBLE, sender, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					
			int run=0;
			for (auto it : function->parameters){
			An.par[it.getname()]=Ad[run];
			run++;
			}
return An;	
}
