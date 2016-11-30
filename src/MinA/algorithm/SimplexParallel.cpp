#include "MinA/algorithm/SimplexParallel.h"
using namespace std;

SimplexParallel::SimplexParallel(int stop) : Simplex(stop)
{
    if (!MPI::Is_initialized())
        throw runtime_error("MPI_INIT must be used beforehand");
}

SimplexParallel::~SimplexParallel() {}

Result SimplexParallel::algorithm(shared_ptr<FunctionToBeOptimized> start)
{

    mFunction = start;
    mDimension = mFunction->getParSpaceDim();
    verticesVector A;

    // restore();
    // cout << "Start with current loop= " << currentIteration << endl;
    setStepSize();
    if (Acopy.empty()) {
        A.resize(mDimension + 1);
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

            for (int i = 0; i <= mDimension; i++) {
                A[i].second = mFunction->evaluate(A[i].first);
                // vector<double> pars;
                // for (auto iPar: A[i].first)
                //     pars.push_back(iPar.second);
                // A[i].second = mFunction->evaluate(pars);
            } // Evaluate(A)
            sort(A.begin(), A.end(),
                 [](vertex& a, vertex& b) -> bool { return a.second < b.second; });
            // cout << "loop= " << currentIteration << endl;
            // printOutVertices(A);

            ofstream dataFile;
            dataFile.open("z_SimplexParallel" + getFunctionName() + to_string(world_size),
                          ios::app);
            dataFile << currentIteration << endl;
            // for (int i = 0; i <= mDimension; i++) {
            //     dataFile << "Iteration= " << currentIteration << "	f(A" << i
            //              << ")= " << A[i].second << "	";
            //     for (map<string, double>::iterator it = A[i].first.begin();
            //          it != A[i].first.end(); ++it)
            //         dataFile << it->first << " = " << it->second << "	";
            //     dataFile << "\n";
            // }
            printOutVertices(A, dataFile);
            dataFile.close();

            vertex M = getCentroid(A, world_size); // Centroid
            M.second = mFunction->evaluate(M.first);
            // vector<double> pars;
            // for (auto iPar: M.first)
            //     pars.push_back(iPar.second);
            // M.second = mFunction->evaluate(pars);
            ofstream dataMean;
            dataMean.open("z_mean" + to_string(world_size), ios::app);
            dataMean << "Iteration= " << currentIteration << "	Mean= " << M.second
                     << "	SimplexSize =	" << getSimplexSize(A) << "	";
            /*for (map<string, double>::iterator it = M.first.begin(); it != M.first.end();
                 ++it)
                dataMean << it->first << " = " << it->second << "	\n";*/
            dataMean << endl;
            dataMean.close();
            mode = 1;
            for (int i = 1; i < world_size; i++) {
                MPI_Send(&mode, 1, MPI_INT, i, world_size, MPI_COMM_WORLD);
                sendVertex(M, i, 0);
                sendVertex(A[0], i, 1);
                sendVertex(A[mDimension - world_size + i], i, 2);
                sendVertex(A[mDimension - world_size + i + 1], i, 3);

            } // sending M,A0,Aj_1,Aj

            int sumcheck = 0;

            for (int i = 1; i < world_size; i++) {
                int check;
                MPI_Recv(&(check), 1, MPI_INT, i, i + 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                sumcheck += check;
            }
            if (sumcheck == 0) {
                for (int i = 1; i < mDimension - world_size + 1; i++) {
                    vertex Anew = getShrinkedPoint(A[i], A[0]);
                    push(A[i], Anew);
                }
                for (int i = 1; i < world_size; i++) {
                    vertex b = receiveVertex(i, i);
                    vertex Anew = getShrinkedPoint(b, A[0]);
                    push(A[mDimension - world_size + i + 1], Anew);
                }
            } // case 4
            else
                for (int i = 1; i < world_size; i++) {
                    vertex b = receiveVertex(i, i);
                    b.second = mFunction->evaluate(b.first);
                    // vector<double> pars;
                    // for (auto iPar: b.first)
                    //     pars.push_back(iPar.second);
                    // b.second = mFunction->evaluate(pars);
                    push(A[mDimension - world_size + i + 1], b);
                }

            dataFile.open("z_SimplexParallel" + getFunctionName() + to_string(world_size),
                          ios::app);
            dataFile << currentIteration << endl;
            printOutVertices(A, dataFile);
            dataFile.close();

            dataFile.open("z_dataParallel" + to_string(world_size), ios::app);
            dataFile << "Iteration= " << currentIteration << "	f(A0)= " << A[0].second << "	";
            printOutVertices(A, dataFile);
            dataFile << "\n";
            dataFile.close();
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
                A0.second = mFunction->evaluate(A0.first);
                // vector<double> pars;
                // for (auto iPar: A0.first)
                //     pars.push_back(iPar.second);
                // A0.second = mFunction->evaluate(pars);
                Aj_1.second = mFunction->evaluate(Aj_1.first);
                // vector<double> pars1;
                // for (auto iPar: Aj_1.first)
                //     pars1.push_back(iPar.second);
                // Aj_1.second = mFunction->evaluate(pars1);
                Aj.second = mFunction->evaluate(Aj.first);
                // vector<double> pars2;
                // for (auto iPar: Aj.first)
                //     pars2.push_back(iPar.second);
                // Aj.second = mFunction->evaluate(pars2);
                int check = 0;

                // Update vertex step3
                Ar = getReflectedPoint(M, Aj);
                if (Ar.second < A0.second) {
                    Ae = getExtendedPoint(M, Ar);
                    if (Ae.second < A0.second) {
                        // push(Aj, Ae);
                        Aj = Ae;
                        check = 1;
                    }
                    else {
                        // push(Aj, Ar);
                        Aj = Ar;
                        check = 2;
                    }
                } // case 1
                else if (Ar.second < Aj_1.second) {
                    // push(Aj, Ar);
                    Aj = Ar;
                    check = 3;
                } // case 2
                else {
                    if (Ar.second < Aj.second) {
                        Ap = Ar;
                    }
                    else {
                        Ap = Aj;
                    } // Ac
                    Ac = getContractedPoint(M, Ap);

                    if (Ac.second < Ap.second) {
                        // if(Ac.second<Aj.second)cout<<"Ac="<<Ac.second<<endl;
                        // else 	cout<<"Aj="<<Aj.second<<endl;
                        // push(Aj, Ac);
                        Aj = Ac;
                        check = 4;
                    } // case 3
                    else
                        // push(Aj, Ap);
                        Aj = Ap;
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
    double Ad[mDimension];
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Ad[iPar] = An.first[iPar];

    MPI_Send(&(Ad[0]), mDimension, MPI_DOUBLE, receiver, tag, MPI_COMM_WORLD);
}

vertex SimplexParallel::receiveVertex(int sender, int tag)
{
    vertex An;
    double Ad[mDimension];
    MPI_Recv(&(Ad[0]), mDimension, MPI_DOUBLE, sender, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    for (int iPar = 0; iPar < mDimension; ++iPar)
        An.first[iPar] = Ad[iPar];

    return An;
}

vertex SimplexParallel::getContractedPoint(vertex& M, vertex& Ajp)
{
    // Return contraction point Ac of initial point Ajp w.r.t. centroid M
    // Ac = beta * (Ajp + M)
    //
    vertex Ac;
    Ac.first.resize(mDimension);
    for (int iPar = 0; iPar < mDimension; ++iPar)
        Ac.first[iPar] = getMetaParameter("beta") * (Ajp.first[iPar] + M.first[iPar]);
    Ac.second = mFunction->evaluate(Ac.first);
    return Ac;
}
