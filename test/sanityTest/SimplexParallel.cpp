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
        
	


	if(world_rank==0){
	int mode;
		int jj=0;
		while(jj<100){
		
			for(int i=0;i<=dimension;i++){A[i].second=function->evaluate(A[i].first);}//Evaluate(A)
			std::sort(A,A+dimension+1,[](vertex  & a, vertex   & b) -> bool{return a.second < b.second; });//Sort
			cout<<"loop= "<<jj<<endl;
			showVertex(A);
	
			vertex M;
			calculateM(A,M,world_size);//Mean
			mode=1;
			for(int i=1;i<world_size;i++){					
				MPI_Send(&mode, 1, MPI_INT, i,world_size, MPI_COMM_WORLD);		
				sendVertex(M,i,0);
				sendVertex(A[0],i,1);
				sendVertex(A[dimension-i],i,2);
				sendVertex(A[dimension-i+1],i,3);
			
			}//sending M,A0,Aj_1,Aj
		
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
			}//case 4
		jj++;		
		}

		mode=0;
		for(int i=1;i<world_size;i++)	
		MPI_Send(&mode, 1, MPI_INT, i,0, MPI_COMM_WORLD);
	}

	 if (world_rank !=0){
		int mode;
		
		while(1){
		MPI_Recv(&(mode), 1, MPI_INT, 0, world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			if(mode==1){
				vertex M,Aj,Aj_1,A0,Ar,Ac,Ae;
				M=receiveVertex(0,0);
				A0=receiveVertex(0,1);	
				Aj_1=receiveVertex(0,2);	
				Aj=receiveVertex(0,3);//receiving M,A0,Aj,Aj_1
		
				//calculate
				A0.second=function->evaluate(A0.first);
				Aj_1.second=function->evaluate(Aj_1.first);
				Aj.second=function->evaluate(Aj.first);
				int check=0;
			
				//Update vertex step3
				calculateAr(Ar,M,Aj);//Ar
				if(Ar.second<A0.second){
					calculateAe(Ae,M,Ar);//Ae
					if(Ae.second<A0.second){push(Aj,Ae);check=1;}
					else{push(Aj,Ar);check=2;}
				}//case 1	
				else if(Ar.second<Aj_1.second){push(Aj,Ar);check=3;}//case 2 
				else {
					if(Ar.second>Aj.second){
						calculateAc(Ac,M,Aj);
					}else{
					calculateAc(Ac,M,Ar);
					}//Ac
					if(Ac.second<Aj.second){		
						push(Aj,Ac);
						check=4;
					}
				}//case 3
			
				sendVertex(Aj,0,world_rank);
				MPI_Send(&(check), 1, MPI_INT, 0, world_rank+1, MPI_COMM_WORLD);
			}
			else if(mode==0) break;	
				
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
		for (std::map<string, double>::iterator it=para[i].first.begin(); it!=para[i].first.end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<para[i].second<<endl;
	}
}

void SimplexParallel::showM(vertex &para){
	
	for (std::map<string, double>::iterator it=para.first.begin(); it!=para.first.end(); ++it)
    		std::cout << it->first << "=" << it->second << " ";	
	cout<<"f(M)="<<para.second<<endl;
	para.second=100;
}


void SimplexParallel::calculateM(vertex *A,vertex &M,int world_size){
	for(int i=0;i<= dimension -world_size+1;i++){
		for(auto it : function->parameters){
			M.first[it.getname()]+=A[i].first[it.getname()];		
		}
	}
	for (auto it : function->parameters){
		M.first[it.getname()]/=(dimension-world_size+2);		
	}
}

void SimplexParallel::calculateAr(vertex &Ar,vertex &M,vertex &Aj){
	for (auto it : function->parameters)
		Ar.first[it.getname()]=M.first[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M.first[it.getname()]-Aj.first[it.getname()]);		
	Ar.second=function->evaluate(Ar.first);
}

void SimplexParallel::calculateAe(vertex &Ae,vertex &M,vertex &Ar){
	for (auto it : function->parameters)
		Ae.first[it.getname()]=Ar.first[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar.first[it.getname()]-M.first[it.getname()]);	
	Ae.second=function->evaluate(Ae.first);			
}

void SimplexParallel::calculateAc(vertex &Ac,vertex &M,vertex &Ajp){
	for (auto it : function->parameters)
	Ac.first[it.getname()]=M.first[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ajp.first[it.getname()]-M.first[it.getname()]);			
	Ac.second=function->evaluate(Ac.first);	
}

void SimplexParallel::push(vertex &a,vertex &b){
	for (auto it : function->parameters)a.first[it.getname()]=b.first[it.getname()];
	a.second=b.second;
}

void SimplexParallel::pushResult(Result &rs,vertex &A){
	for (auto it : function->parameters)
		rs.optimizationparameter["it.getname()"]=A.first[it.getname()];
		rs.result=A.second;	
}

void SimplexParallel::newVertex(vertex *A){
	for(int i=1;i<=dimension;i++)
		for (auto it : function->parameters)	
			A[i].first[it.getname()]=GetOptimizationAlgorithmParameter("tau")*A[0].first[it.getname()]+(1-GetOptimizationAlgorithmParameter("tau"))*A[i].first[it.getname()];

}

void SimplexParallel::InitialVertex(vertex *A){
	for (auto it : function->parameters)
		A[0].first[it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=function->parameters.begin(); it!=function->parameters.end(); ++it){
			if(std::distance(function->parameters.begin(),it)==i-1)
				A[i].first[it->getname()]=A[0].first[it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i].first[it->getname()]=A[0].first[it->getname()];
		}

	}
}
void SimplexParallel::sendVertex(vertex &An,int receiver,int tag){
			double Ad[dimension];	
			int run=0;
			for (auto it : function->parameters){
			Ad[run]=An.first[it.getname()];		
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
			An.first[it.getname()]=Ad[run];
			run++;
			}
return An;	
}
