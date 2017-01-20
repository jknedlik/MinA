#include "MinA/common/test.hpp"
<typename CON, typename IDENT> class Communicator {
    CON context;
    IDENT unique_id;
    template <typename T>
    void send(IDENT destination, T t) = 0;
    template <typename T>
    T receive(IDENT source) = 0;
    template <typename T>
    void broadcast(T t) = 0;
};
class MPIComm : public Comunicator<size_t, size_t> {
    MPIComm(){};
    initalize(size_t context){

    };
};
