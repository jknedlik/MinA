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
        if (!this->restored) {
            this->initializeVertices(A); // Initialze A
        }
        else {
            A = std::get<SIMPLEX_AI_CURR_A>(this->mAlgorithmInformations);
        }

        MinA::Communicator<MinA::MPIContext> all(this->mpi_procs * this->f.mpi_procs);
        if (all) {
            if (all == 0) {
                int mode;

                while (this->checkStoppingCondition()) {
                    // Initial evaluation
                    for (auto a : A) {
                        a.second = this->f.evaluate(a.first);
                    }
                    sort(A.begin(), A.end(), [](vertex<Function>& a, vertex<Function>& b) -> bool {
                        return a.second < b.second;
                    });

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
                    M.second = this->f.evaluate(M.first);

                    ofstream meanFile;
                    string outFile_mean("parallelSimplex_" + to_string(all.getSize()) + "_Size");
                    meanFile.open(outFile_mean, ios::app);
                    meanFile << "Iteration= "
                             << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
                             << "	Mean= " << M.second
                             << "	SimplexSize =	" << this->getSimplexSize(A) << "	";
                    meanFile << endl;
                    meanFile.close();
                    mode = 1;
                    for (int i = 1; i < all.getSize(); i++) {
                        all.send(i, std::tie(mode));
                        all.send(i, M.first);
                        all.send(i, A[0].first);
                        all.send(i, A[mDimension - all.getSize() + i].first);
                        all.send(i, A[mDimension - all.getSize() + i + 1].first);
                    } // sending M,A0,Aj_1,Aj

                    int sumcheck = 0;

                    for (int i = 1; i < all.getSize(); i++) {
                        int check = std::get<0>(all.receive<std::tuple<int>>(i));
                        // MPI_Recv(&(check), 1, MPI_INT, i, i + 1, MPI_COMM_WORLD,
                        // MPI_STATUS_IGNORE);

                        sumcheck += check;
                    }
                    if (sumcheck == 0) {
                        for (int i = 1; i < mDimension - all.getSize() + 1; i++) {
                            A[i] = this->getShrinkedPoint(A[i], A[0]);
                        }
                        for (int i = 1; i < all.getSize(); i++) {
                            vertex<Function> b;
                            b.first = all.receive<decltype(b.first)>(i);
                            A[mDimension - all.getSize() + i + 1] = this->getShrinkedPoint(b, A[0]);
                        }
                    } // case 4
                    else
                        for (int i = 1; i < all.getSize(); i++) {
                            vertex<Function> b;
                            b.first = all.receive<decltype(b.first)>(i);
                            b.second = this->f.evaluate(b.first);
                            A[mDimension - all.getSize() + i + 1] = b;
                        }

                    ofstream fValueFile;
                    string outFile_fValue("parallelSimplex_" + to_string(all.getSize()) +
                                          "_fValue");
                    fValueFile.open(outFile_fValue, ios::app);
                    fValueFile << "Iteration= "
                               << std::get<SIMPLEX_CURR_ITERATIONS>(this->mAlgorithmInformations)
                               << "   ";
                    this->printOutVertex(A[0], "A[0]", fValueFile);
                    fValueFile.close();
                    // save();
                }

                mode = 0;
                for (int i = 1; i < all.getIdent(); i++) {
                    //  MPI_Send(&mode, 1, MPI_INT, i, all.getIdent, MPI_COMM_WORLD);
                    all.send(i, std::tie(mode));
                }
            }

            else {

                while (1) {
                    int mode;
                    // MPI_Recv(&mode, 1, MPI_INT, 0, all.getIdent(), MPI_COMM_WORLD,
                    //         MPI_STATUS_IGNORE);
                    mode = std::get<0>(all.receive<std::tuple<int>>(0));
                    if (mode == 1) {
                        vertex<Function> M, Aj, Aj_1, A0, Ar, Ac, Ae, Ap;
                        // receiving M,A0,Aj,Aj_1,should rather use scatter/broadcast
                        M.first = all.receive<decltype(M.first)>(0);
                        A0.first = all.receive<decltype(A0.first)>(0);
                        Aj_1.first = all.receive<decltype(Aj_1.first)>(0);
                        Aj.first = all.receive<decltype(Aj.first)>(0);
                        A0.second = this->f.evaluate(A0.first);
                        Aj_1.second = this->f.evaluate(Aj_1.first);
                        Aj.second = this->f.evaluate(Aj.first);
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
                                // if(Ac.second<Aj.second)cout<<"Ac="<<Ac.second<<endl;
                                // else 	cout<<"Aj="<<Aj.second<<endl;
                                Aj = Ac;
                                check = 4;
                            } // case 3
                            else
                                Aj = Ap;
                        }

                        // sendVertex(Aj, 0, all.getIdent());
                        all.send(0, Aj.first);
                        all.send(0, std::tie(check));
                    }
                    else if (mode == 0) {
                        MinA::Result<typename Function::parametertype> r;
                        return r;
                    }
                }
            }
        }
        // real ugly leftover from v1, needs to be refactored
        if (all.getIdent() == 0) {
            MinA::Result<typename Function::parametertype> r;
            r.parameters = A[0].first;
            r.value = A[0].second;
            // Result
            return r;
        }
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
};
}
#endif
