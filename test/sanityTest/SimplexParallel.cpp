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
	

	dimension=start->getparametersize();
	vertex A[dimension+1];

			
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

	MPI_Init(NULL, NULL);
  	int world_rank;
  	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        
	for(int jj=0;jj<100;jj++){

	if(world_rank==0){
	//Sort

		for(int i=0;i<=dimension;i++){A[i].func=start->evaluate(A[i].par);}
		std::sort(A,A+dimension+1,[](vertex  & a, vertex   & b) -> bool{
		return a.func < b.func; });
	cout<<"jj= "<<jj<<endl;
	showfunc(A);

	//Mean
		vertex M;
		for(int i=0;i<=dimension-world_size+1;i++){
			for(auto it : start->parameters){
				M.par[it.getname()]+=A[i].par[it.getname()];		
			}
		}
		for (auto it : start->parameters){
			M.par[it.getname()]/=(dimension-world_size+2);		
		}

	//sending M,A0,Aj_1,Aj	
		double box[4][dimension];
		int run=0;
		for (auto it : start->parameters){
			box[1][run]=A[0].par[it.getname()];		
			box[0][run]=M.par[it.getname()];
			run++;
		}
		for(int i=1;i<world_size;i++){
			run=0;
			for (auto it : start->parameters){
			box[2][run]=A[dimension-i].par[it.getname()];		
			box[3][run]=A[dimension-i+1].par[it.getname()];
			run++;
			}
			MPI_Send(&(box[0][0]), 4*dimension, MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
		}

	}

        if (world_rank !=0){
	//Receiving
	 double box[4][dimension];
   	 MPI_Recv(&(box[0][0]), 4*dimension, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	//cout<<"world_rank="<<world_rank<<endl;	

	vertex M,Aj,Aj_1,A0,Ar,Ac,Ae;
	int run=0;
	for (auto it : start->parameters){
			M.par[it.getname()]=box[0][run];		
			A0.par[it.getname()]=box[1][run];
			Aj_1.par[it.getname()]=box[2][run];			
			Aj.par[it.getname()]=box[3][run];
			run++;
		}
	//calculate
		A0.func=start->evaluate(A0.par);
		Aj_1.func=start->evaluate(Aj_1.par);
		Aj.func=start->evaluate(Aj.par);
		int check=0;
		//Ar
		for (auto it : start->parameters){
			Ar.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M.par[it.getname()]-Aj.par[it.getname()]);		
		}
		Ar.func=start->evaluate(Ar.par);
		//Ae
		for (auto it : start->parameters){
			Ae.par[it.getname()]=Ar.par[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar.par[it.getname()]-M.par[it.getname()]);		
		}
		//Ac
		if(Ar.func>Aj.func){
			for (auto it : start->parameters){
				Ac.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Aj.par[it.getname()]-M.par[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac.par[it.getname()]=M.par[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar.par[it.getname()]-M.par[it.getname()]);			
			}
		}

		
		
		Ac.func=start->evaluate(Ac.par);
		Ae.func=start->evaluate(Ae.par);
	//case//Update vertex step3
		//case 1
		if(Ar.func<A0.func){
			if(Ae.func<A0.func)
				for (auto it : start->parameters){Aj.par[it.getname()]=Ae.par[it.getname()];check=1;}
			else
				for (auto it : start->parameters){Aj.par[it.getname()]=Ar.par[it.getname()];check=2;}
		}	
		//case 2
		else if(Ar.func<Aj_1.func)
			for (auto it : start->parameters){Aj.par[it.getname()]=Ar.par[it.getname()];check=3;}
		//case 3
		else if(Ac.func<Aj.func)
			for (auto it : start->parameters){Aj.par[it.getname()]=Ac.par[it.getname()];check=4;}
		
			double Ajd[dimension];
	
			run=0;
			for (auto it : start->parameters){
			Ajd[run]=Aj.par[it.getname()];		
			run++;
			}
			/*for (auto it : start->parameters)
    			std::cout << Aj[it.getname()]<< " ";	
			cout<<"jj= "<<jj<<" check= "<<check<<" f(Aj)"<<"="<<start->evaluate(Aj)<<"world_rank= "<<world_rank<<" send data to World_rank=0"<<endl;*/

			MPI_Send(&(Ajd[0]), dimension, MPI_DOUBLE, 0, world_rank, MPI_COMM_WORLD);
			MPI_Send(&(check), 1, MPI_INT, 0, world_rank+1, MPI_COMM_WORLD);
		}
	
	
	
/////////////////////////////////////////////////////////////
	if(world_rank==0){
		
		int sumcheck=0;
		for(int i=1;i<world_size;i++){
			double Ajd[dimension];
			int check;
			MPI_Recv(&(Ajd[0]), dimension, MPI_DOUBLE, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&(check), 1, MPI_INT, i, i+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			/*cout<<"jj= "<<jj<<" check= "<<check<<"world_rank=0 receive from world_rank="<<i<<" ";
			for(int j=0;j<dimension;j++)
				cout<<Ajd[j]<<" ";
			cout<<endl;*/
			sumcheck+=check; 		
			int run=0;
			for (auto it : start->parameters){
			A[dimension-i+1].par[it.getname()]=Ajd[run];
			run++;
			}
			
		}
		//cout<<"sumcheck= "<<sumcheck<<"jj= "<<jj<<endl;
		//case 4
		//sumcheck=1;		
		if(sumcheck==0){
			for(int i=1;i<=dimension;i++)
				for (auto it : start->parameters)	
					A[i].par[it.getname()]=GetOptimizationAlgorithmParameter("tau")*A[0].par[it.getname()]+(1-GetOptimizationAlgorithmParameter("tau"))*A[i].par[it.getname()];
		}	
		//showfunc(start,A);
	}
}
	
	if(world_rank==0){		
	//Return result
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0].par[it.getname()];
		rs.result=A[0].func;
	
	return 	rs;
	}
	 MPI_Finalize();	
}

void SimplexParallel::showfunc(vertex *para){
	
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].par.begin(); it!=para[i].par.end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<para[i].func<<endl;
	}
}
