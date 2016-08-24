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
	

	int dimension=start->getparametersize();
	std::map <string, double> A[dimension+1];

			
	//Initial
	
	
	for (auto it : start->parameters)
		A[0][it.getname()]=it.getstartingPoint();


	for(int i=1;i<=dimension;i++){
		for ( std::set<Parameter>::iterator it=start->parameters.begin(); it!=start->parameters.end(); ++it){
			if(std::distance(start->parameters.begin(),it)==i-1)
				A[i][it->getname()]=A[0][it->getname()]+GetOptimizationAlgorithmParameter("stepsize");
			else
				A[i][it->getname()]=A[0][it->getname()];
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
		std::sort(A,A+dimension+1,[start](std::map <string, double>  & a, std::map <string, double>  & b) -> bool{
		return start->evaluate(a) < start->evaluate(b) ; });
	cout<<"jj= "<<jj<<endl;
	showfunc(start,A);

	//Mean
	std::map <string, double> M;
		for(int i=0;i<=dimension-world_size+1;i++){
			for(auto it : start->parameters){
				M[it.getname()]+=A[i][it.getname()];		
			}
		}
		for (auto it : start->parameters){
			M[it.getname()]/=(dimension-world_size+2);		
		}

	//sending M,A0,Aj_1,Aj	
		double box[4][dimension];
		int run=0;
		for (auto it : start->parameters){
			box[1][run]=A[0][it.getname()];		
			box[0][run]=M[it.getname()];
			run++;
		}
		for(int i=1;i<world_size;i++){
			run=0;
			for (auto it : start->parameters){
			box[2][run]=A[dimension-i][it.getname()];		
			box[3][run]=A[dimension-i+1][it.getname()];
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

	std::map <string, double> M,Aj,Aj_1,A0,Ar,Ac,Ae;
	int run=0;
	for (auto it : start->parameters){
			M[it.getname()]=box[0][run];		
			A0[it.getname()]=box[1][run];
			Aj_1[it.getname()]=box[2][run];			
			Aj[it.getname()]=box[3][run];
			run++;
		}
	//calculate
		
		int check=0;
		//Ar
		for (auto it : start->parameters){
			Ar[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("alpha")*(M[it.getname()]-Aj[it.getname()]);		
		}
		//Ae
		for (auto it : start->parameters){
			Ae[it.getname()]=Ar[it.getname()]+GetOptimizationAlgorithmParameter("gramma")*(Ar[it.getname()]-M[it.getname()]);		
		}
		//Ac
		if(start->evaluate(Ar)>start->evaluate(Aj)){
			for (auto it : start->parameters){
				Ac[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Aj[it.getname()]-M[it.getname()]);			
			}
		}
		else{
			for (auto it : start->parameters){
				Ac[it.getname()]=M[it.getname()]+GetOptimizationAlgorithmParameter("beta")*(Ar[it.getname()]-M[it.getname()]);			
			}
		}


	//case//Update vertex step3
		//case 1
		if(start->evaluate(Ar)<start->evaluate(A0)){
			if(start->evaluate(Ae)<start->evaluate(A0))
				for (auto it : start->parameters){Aj[it.getname()]=Ae[it.getname()];check=1;}
			else
				for (auto it : start->parameters){Aj[it.getname()]=Ar[it.getname()];check=2;}
		}	
		//case 2
		else if(start->evaluate(Ar)<start->evaluate(Aj_1))
			for (auto it : start->parameters){Aj[it.getname()]=Ar[it.getname()];check=3;}
		//case 3
		else if(start->evaluate(Ac)<start->evaluate(Aj))
			for (auto it : start->parameters){Aj[it.getname()]=Ac[it.getname()];check=4;}
		
			double Ajd[dimension];
	
			run=0;
			for (auto it : start->parameters){
			Ajd[run]=Aj[it.getname()];		
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
			A[dimension-i+1][it.getname()]=Ajd[run];
			run++;
			}
			
		}
		//cout<<"sumcheck= "<<sumcheck<<"jj= "<<jj<<endl;
		//case 4
		//sumcheck=1;		
		if(sumcheck==0){
			for(int i=1;i<=dimension;i++)
				for (auto it : start->parameters)	
					A[i][it.getname()]=GetOptimizationAlgorithmParameter("tau")*A[0][it.getname()]+(1-GetOptimizationAlgorithmParameter("tau"))*A[i][it.getname()];
		}	
		//showfunc(start,A);
	}
}
	
	if(world_rank==0){		
	//Return result
	Result rs;
		for (auto it : start->parameters)
		rs.optimizationparameter["it.getname()"]=A[0][it.getname()];
		rs.result=start->evaluate(A[0]);
	
	return 	rs;
	}
	 MPI_Finalize();	
}

void SimplexParallel::showfunc(Functiontobeoptimized* start,std::map <string, double> *para){
	int dimension=start->getparametersize();
	for(int i=0;i<=dimension;i++){
		for (std::map<string, double>::iterator it=para[i].begin(); it!=para[i].end(); ++it)
    			std::cout << it->first << "=" << it->second << " ";	
		cout<<"f(A"<<i<<")="<<start->evaluate(para[i])<<endl;
	}
}
