#include "MinA/algorithm/SimplexParallel.h"

SimplexParallel::SimplexParallel(int stop) : Simplex(stop)
{
    if (!MPI::Is_initialized())
        throw std::runtime_error("MPI_INIT must be used beforehand");
}

SimplexParallel::~SimplexParallel() {}

Result SimplexParallel::algorithm(std::shared_ptr<FunctionToBeOptimized> start)
{

    function = start;
    dimension = function->getParSpaceDim();
    vertexVector A;

    // restore();
    // cout << "Start with current loop= " << currentIteration << endl;
    setStepSize();
    if (Acopy.empty()) {
        A.resize(dimension + 1);
        initializeVertices(A); // Initial
    }
    else {
        A = Acopy;
    }

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    if (world_rank == 0) {
        int mode;

        while (checkStoppingCondition()) {

            for (int i = 0; i <= dimension; i++) {
                //A[i].second = function->evaluate(A[i].first);
                vector<double> pars;
                for (auto iPar: A[i].first)
                    pars.push_back(iPar.second);
                    A[i].second = function->evaluate(pars);
            } // Evaluate(A)
            std::sort(A.begin(), A.end(),
                      [](vertex& a, vertex& b) -> bool { return a.second < b.second; });
            // cout << "loop= " << currentIteration << endl;
            // showVertex(A);

            ofstream dataFile;
            dataFile.open("z_SimplexParallel" + getFunctionName() + std::to_string(world_size),
                          std::ios::app);
            dataFile << currentIteration << std::endl;
            for (int i = 0; i <= dimension; i++) {
                dataFile << "Iteration= " << currentIteration << "	f(A" << i
                         << ")= " << A[i].second << "	";
                for (std::map<string, double>::iterator it = A[i].first.begin();
                     it != A[i].first.end(); ++it)
                    dataFile << it->first << " = " << it->second << "	";
                dataFile << "\n";
            }
            dataFile.close();

            vertex M;

            calculateM(A, M, world_size); // Mean
            //M.second = function->evaluate(M.first);
            vector<double> pars;
            for (auto iPar: M.first)
                pars.push_back(iPar.second);
            M.second = function->evaluate(pars);
            ofstream dataMean;
            dataMean.open("z_mean" + std::to_string(world_size), std::ios::app);
            dataMean << "Iteration= " << currentIteration << "	Mean= " << M.second
                     << "	SimplexSize =	" << getSimplexSize(A) << "	";
            /*for (std::map<string, double>::iterator it = M.first.begin(); it != M.first.end();
                 ++it)
                dataMean << it->first << " = " << it->second << "	\n";*/
            dataMean << std::endl;
            dataMean.close();
            mode = 1;
            for (int i = 1; i < world_size; i++) {
                MPI_Send(&mode, 1, MPI_INT, i, world_size, MPI_COMM_WORLD);
                sendVertex(M, i, 0);
                sendVertex(A[0], i, 1);
                sendVertex(A[dimension - world_size + i], i, 2);
                sendVertex(A[dimension - world_size + i + 1], i, 3);

            } // sending M,A0,Aj_1,Aj

            int sumcheck = 0;

            for (int i = 1; i < world_size; i++) {
                int check;
                MPI_Recv(&(check), 1, MPI_INT, i, i + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                sumcheck += check;
            }
            if (sumcheck == 0) {
                for (int i = 1; i < dimension - world_size + 1; i++) {
                    vertex Anew;
                    calculateNewPoint(Anew, A[i], A[0]);
                    push(A[i], Anew);
                }
                for (int i = 1; i < world_size; i++) {
                    vertex b = receiveVertex(i, i);
                    vertex Anew;
                    calculateNewPoint(Anew, b, A[0]);
                    push(A[dimension - world_size + i + 1], Anew);
                }
            } // case 4
            else
                for (int i = 1; i < world_size; i++) {
                    vertex b = receiveVertex(i, i);
                    //b.second = function->evaluate(b.first);
                    vector<double> pars;
                    for (auto iPar: b.first)
                        pars.push_back(iPar.second);
                    b.second = function->evaluate(pars);
                    push(A[dimension - world_size + i + 1], b);
                }

            ofstream dataFile2;
            dataFile.open("z_SimplexParallel" + getFunctionName() + std::to_string(world_size),
                          std::ios::app);
            dataFile << currentIteration << std::endl;
            for (int i = 0; i <= dimension; i++) {
                dataFile << "Iteration= " << currentIteration << "	f(A" << i
                         << ")= " << A[i].second << "	";
                for (std::map<string, double>::iterator it = A[i].first.begin();
                     it != A[i].first.end(); ++it)
                    dataFile << it->first << " = " << it->second << "	";
                dataFile << "\n";
            }
            dataFile2.close();
            dataFile2.open("z_dataParallel" + std::to_string(world_size), std::ios::app);
            dataFile2 << "Iteration= " << currentIteration << "	f(A0)= " << A[0].second << "	";
            for (std::map<string, double>::iterator it = A[0].first.begin(); it != A[0].first.end();
                 ++it)
                dataFile2 << it->first << " = " << it->second << "	";
            dataFile2 << "\n";
            dataFile2.close();
            // save();
        }

        mode = 0;
        for (int i = 1; i < world_size; i++) {
            MPI_Send(&mode, 1, MPI_INT, i, world_size, MPI_COMM_WORLD);
        }
    }

    if (world_rank != 0) {

        while (1) {
            int mode;
            MPI_Recv(&mode, 1, MPI_INT, 0, world_size, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (mode == 1) {
                vertex M, Aj, Aj_1, A0, Ar, Ac, Ae, Ap;
                M = receiveVertex(0, 0);
                A0 = receiveVertex(0, 1);
                Aj_1 = receiveVertex(0, 2);
                Aj = receiveVertex(0, 3); // receiving M,A0,Aj,Aj_1
                //A0.second = function->evaluate(A0.first);
                vector<double> pars;
                for (auto iPar: A0.first)
                    pars.push_back(iPar.second);
                A0.second = function->evaluate(pars);
                //Aj_1.second = function->evaluate(Aj_1.first);
                vector<double> pars1;
                for (auto iPar: Aj_1.first)
                    pars1.push_back(iPar.second);
                Aj_1.second = function->evaluate(pars1);
                //Aj.second = function->evaluate(Aj.first);
                vector<double> pars2;
                for (auto iPar: Aj.first)
                    pars2.push_back(iPar.second);
                Aj.second = function->evaluate(pars2);
                int check = 0;

                // Update vertex step3
                calculateAr(Ar, M, Aj); // Ar
                if (Ar.second < A0.second) {
                    calculateAe(Ae, M, Ar); // Ae
                    if (Ae.second < A0.second) {
                        push(Aj, Ae);
                        check = 1;
                    }
                    else {
                        push(Aj, Ar);
                        check = 2;
                    }
                } // case 1
                else if (Ar.second < Aj_1.second) {
                    push(Aj, Ar);
                    check = 3;
                } // case 2
                else {
                    if (Ar.second < Aj.second) {
                        Ap = Ar;
                    }
                    else {
                        Ap = Aj;
                    } // Ac
                    calculateAc(Ac, M, Ap);

                    if (Ac.second < Ap.second) {
                        // if(Ac.second<Aj.second)cout<<"Ac="<<Ac.second<<endl;
                        // else 	cout<<"Aj="<<Aj.second<<endl;
                        push(Aj, Ac);
                        check = 4;
                    } // case 3
                    else
                        push(Aj, Ap);
                }

                sendVertex(Aj, 0, world_rank);
                MPI_Send(&(check), 1, MPI_INT, 0, world_rank + 1, MPI_COMM_WORLD);
            }
            else if (mode == 0) {
                Result rs;
                return rs;
            }
        }
    }

    if (world_rank == 0) {

        Result rs;
        pushResult(rs, A[0]); // Result
        return rs;
    }
    Result rs;
    return rs;
}

void SimplexParallel::sendVertex(vertex& An, int receiver, int tag)
{
    double Ad[dimension];
    int run = 0;
    for (auto it : function->mParameters) {
        Ad[run] = An.first[it.getName()];
        run++;
    }

    MPI_Send(&(Ad[0]), dimension, MPI_DOUBLE, receiver, tag, MPI_COMM_WORLD);
}

vertex SimplexParallel::receiveVertex(int sender, int tag)
{
    vertex An;
    double Ad[dimension];
    MPI_Recv(&(Ad[0]), dimension, MPI_DOUBLE, sender, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    int run = 0;
    for (auto it : function->mParameters) {
        An.first[it.getName()] = Ad[run];
        run++;
    }
    return An;
}

void SimplexParallel::calculateAc(vertex& Ac, vertex& M, vertex& Ajp)
{
    for (auto it : function->mParameters)
        Ac.first[it.getName()] = getOptimizationAlgorithmParameter("beta") *
                                 (Ajp.first[it.getName()] + M.first[it.getName()]);
    //Ac.second = function->evaluate(Ac.first);
    vector<double> pars;
    for (auto iPar: Ac.first)
        pars.push_back(iPar.second);
    Ac.second = function->evaluate(pars);
}
