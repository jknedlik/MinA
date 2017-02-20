#ifndef MINA_PSIMPLEX
#define MINA_PSIMPLEX
#include "MinA/algorithm/SimplexT.h"
#include "MinA/common/communicator.hpp"
namespace MinA {
template <typename Function>
using vertex = std::pair<typename Function::parametertype, double>;
template <typename Function>
using simplext =
  std::array<vertex<Function>, std::tuple_size<decltype(Function::startvalues)>::value + 1>;
template <typename Function>
using ai = std::tuple<size_t, size_t, typename Function::parametertype, simplext<Function>>;
template <typename Function>
class ParallelSimplex : public Simplex<Function> {
  public:
    ParallelSimplex() : Simplex<Function>(){};
    Result<typename Function::parametertype> run()
    {

        constexpr size_t mDimension = std::tuple_size<typename Function::parametertype>::value;

        simplext<Function> A;
        this->setStepSize();
        this->restore();
        bool initialized = this->restored;
        if (!this->restored) {
            this->initializeVertices(A); // Initialze A
        }
        else {
            A = std::get<SIMPLEX_AI_CURR_A>(this->mAlgorithmInformations);
        }

        std::cout << "not created all context " << std::endl;
        MinA::Communicator<MinA::MPIContext> all(this->mpi_procs * this->f.mpi_procs);
        std::cout << "created all context " << all.getIdent() << std::endl;
        if (all) {
            if (all == 0) {
                int mode;

                while (this->checkStoppingCondition()) {
                    // Initial evaluation
                    if (!initialized) {
                        evalSimplex(A, all);
                    }
                    ofstream verticesFile;
                    string outFile_vertices(this->filename + "parallelSimplex_" +
                                            std::to_string(all.getIdent()) +
                                            to_string(all.getSize()) + "_Vertices");
                    verticesFile.open(outFile_vertices, ios::app);
                    verticesFile << "  Iteration: "
                                 << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
                                 << endl;
                    this->printOutVertices(A, verticesFile);
                    verticesFile << endl;
                    verticesFile.close();

                    // Centroid of the simplex, excluding world_size vertices
                    vertex<Function> M = this->getCentroid(A);

                    ofstream meanFile;
                    string outFile_mean(this->filename + "parallelSimplex_" +
                                        to_string(all.getSize()) + "_Size");
                    meanFile.open(outFile_mean, ios::app);
                    meanFile << "Iteration= "
                             << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
                             << "	Mean= " << M.second
                             << "	SimplexSize =	" << this->getSimplexSize(A) << "	";
                    meanFile << endl;
                    meanFile.close();
                    mode = 1;
                    for (int i = 1; i < all.getSize(); i++) {
                        sendMode(1, all, i);
                        sendVertex(M, i, all);
                        sendVertex(A[0], i, all);
                        sendVertex(A[mDimension - all.getSize() + i], i, all);
                        sendVertex(A[mDimension - all.getSize() + i + 1], i, all);

                    } // sending M,A0,Aj_1,Aj

                    int sumcheck = 0;

                    for (int i = 1; i < all.getSize(); i++) {

                        // MPI_Recv(&(check), 1, MPI_INT, i, i + 1, MPI_COMM_WORLD,
                        // MPI_STATUS_IGNORE);

                        sumcheck += receiveMode(i, all);
                    }
                    if (sumcheck == 0) {
                        for (int i = 1; i < mDimension - all.getSize() + 1; i++) {
                            A[i] = this->getShrinkedPoint(A[i], A[0]);
                            //  sendMode(2, all, i);
                            //  sendVertex(A[i], i, all);
                            //  sendVertex(A[0], i, all);
                        }
                        for (int i = 1; i < all.getSize(); i++) {
                            vertex<Function> b;
                            b = receiveVertex(i, all);
                            A[mDimension - all.getSize() + i + 1] = this->getShrinkedPoint(
                              b, A[0]); // this part needs to be done in parallel
                        }
                        evalSimplex(A, all);
                    } // case 4
                    else
                        for (int i = 1; i < all.getSize(); i++) {
                            A[mDimension - all.getSize() + i + 1] = receiveVertex(i, all);
                        }

                    ofstream fValueFile;
                    string outFile_fValue(this->filename + "parallelSimplex_" +
                                          to_string(all.getSize()) + "_fValue");
                    fValueFile.open(outFile_fValue, ios::app);
                    fValueFile << "Iteration= "
                               << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
                               << "   ";
                    this->printOutVertex(A[0], "A[0]", fValueFile);
                    fValueFile.close();
                    // save();
                }

                mode = 0;

                MinA::Result<typename Function::parametertype> r;
                r.parameters = A[0].first;
                r.value = A[0].second;
                for (int i = 1; i < all.getSize(); i++) {
                    sendMode(0, all, i);
                    sendVertex(A[0], i, all);
                }
                // Result
                return r;
            }

            else {

                while (1) {

                    // MPI_Recv(&mode, 1, MPI_INT, 0, all.getIdent(), MPI_COMM_WORLD,
                    //         MPI_STATUS_IGNORE);

                    int mode = receiveMode(0, all);
                    if (mode == 1) {

                        vertex<Function> M, Aj, Aj_1, A0, Ar, Ac, Ae, Ap;
                        M = receiveVertex(0, all);
                        A0 = receiveVertex(0, all);
                        Aj_1 = receiveVertex(0, all);
                        Aj = receiveVertex(0, all);

                        int check = 0;

                        // Update vertex step3
                        Ar = this->getReflectedPoint(M, Aj);
                        if (Ar.second < A0.second) {
                            Ae = this->getExtendedPoint(M, Ar);
                            if (Ae.second < A0.second) {
                                Aj = Ae;
                                check = 1;
                            }
                            else {
                                Aj = Ar;
                                check = 2;
                            }
                        } // case 1
                        else if (Ar.second < Aj_1.second) {
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
                            Ac = this->getContractedPoint(M, Ap);

                            if (Ac.second < Ap.second) {
                                Aj = Ac;
                                check = 4;
                            } // case 3
                            else
                                Aj = Ap;
                        }
                        sendMode(check, all, 0);
                        sendVertex(Aj, 0, all);
                    }

                    if (mode == 0) {

                        MinA::Result<typename Function::parametertype> r;
                        vertex<Function> v = receiveVertex(0, all);
                        r.parameters = v.first;
                        r.value = v.second;
                        return r;
                    }
                    if (mode == 2) {
                        vertex<Function> v = receiveVertex(0, all);
                        v.second = this->f.evaluate(v.first);
                        sendVertex(v, 0, all);
                    }
                }
            }
        }
        // real ugly leftover from v1, needs to be refactored

        MinA::Result<typename Function::parametertype> r;
        return r;
    }

    vertex<Function> getContractedPoint(vertex<Function>& M, vertex<Function>& Ajp)
    {
        // Return contraction point Ac of initial point Ajp w.r.t. centroid M
        // Ac = beta * (Ajp + M)
        //
        vertex<Function> Ac;

        for_each_tuple(Ac.first, Ajp.first, M.first, [this](auto& ac, auto& ajp, auto& m) {
            ac = std::get<SIMPLEXT_BETA>(this->mMetaParameters) * (ajp + m);
        });

        Ac.second = this->f.evaluate(Ac.first);
        return Ac;
    }
    void sendVertex(vertex<Function>& v, int u_id, MinA::Communicator<MinA::MPIContext>& all)
    {
        all.send(u_id, v.first);
        all.send(u_id, std::make_tuple(v.second));
    }
    vertex<Function> receiveVertex(int u_id, MinA::Communicator<MinA::MPIContext>& all)
    {
        vertex<Function> v;
        v.first = all.receive<decltype(v.first)>(u_id);
        auto x = all.receive<std::tuple<double>>(u_id);
        v.second = std::get<0>(x);
        return v;
    }
    void sendMode(int i, MinA::Communicator<MinA::MPIContext>& all, int u_id)
    {
        std::cout << all.getIdent() << " sent mode:" << i << std::endl;
        all.send(u_id, std::make_tuple(i));
    }
    int receiveMode(int u_id, MinA::Communicator<MinA::MPIContext>& all)
    {

        std::tuple<int> x = all.receive<std::tuple<int>>(u_id);

        return std::get<0>(x);
    }
    void evalSimplex(simplext<Function>& A, MinA::Communicator<MinA::MPIContext>& all)
    {
        for (int i = A.size(); i > 0; i--) {
            if (i % all.getSize() == 0)
                A[i - 1].second = this->f.evaluate(A[i - 1].first);
            else {
                sendMode(2, all, i % all.getSize());
                sendVertex(A[i - 1], i % all.getSize(), all);
            }
        }
        for (int i = A.size(); i > 0; i--) {
            if (i % all.getSize() != 0)
                A[i - 1] = receiveVertex(i % all.getSize(), all);
        }
        sort(A.begin(), A.end(),
             [](vertex<Function>& a, vertex<Function>& b) -> bool { return a.second < b.second; });
    }
};
}
#endif
