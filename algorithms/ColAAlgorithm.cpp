//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "ColAAlgorithm.h"

#include <sstream>
#include <boost/mpi.hpp>
#include <boost/serialization/shared_ptr.hpp>

using namespace std;

bool ColAAlgorithm::splitAInRowGroups() {
    return false;
}

void ColAAlgorithm::step4_redistributeMatrixA() {

    vector<boost::mpi::request> requests(0);

    // j-th process gets data from c other processes:
    // (j % (p/c)) * c ...... ((j % (p/c)  + 1) * c) - 1
    const int firstSourceIncl = ( j() % (p()/c()) )   *   c();
    const int lastSourceExcl  = firstSourceIncl + c();
    vector<shared_ptr<SparseMatrix>> myFragments((size_t) c());

    int fragmentIdx = 0;
    for (int source = firstSourceIncl; source < lastSourceExcl; ++source) {
        myFragments[fragmentIdx] = make_shared<SparseMatrix>(A->size());
        requests.push_back(
                world.irecv(source, REDISTRIBUTE_A, *myFragments[fragmentIdx])
        );
//        cout << world.rank() << " <== " << source << endl;
        ++fragmentIdx;
    }

    // j-th process sends data to c other processes:
    // (j/c), (j/c) + (p/c), (j/c) + 2(p/c), ..., (j/c) + (c-1)*(p/c)
    const int firstTarget = j() / c();
    const int targetStep = p()/ c();
    const int numSteps = c();
    int target = firstTarget;
    for (int i = 0; i < numSteps; ++i) {
        requests.push_back(
                world.isend(target, REDISTRIBUTE_A, *A)
        );
//        cout << world.rank() << " --> " << target << endl;
        target += targetStep;
    }

    boost::mpi::wait_all(requests.begin(), requests.end());

//    cout << world.rank() << "End of waiting" << endl;

    A = SparseMatrixFragment::mergeCols(myFragments);

//    for (int i = 0; i < world.size(); ++i) {
//        world.barrier();
//        if (i == world.rank()) {
////            cout << "Fragments of process #" << world.rank() << endl;
////            for (int fIdx = 0; fIdx < (int) myFragments.size(); ++fIdx) {
////                cout << *myFragments
//            cout << "Merged fragment of process #" << world.rank() << ":" << endl;
//            cout << *A << endl;
//        }
//        world.barrier();
//    }
}

void ColAAlgorithm::step6_performSingleMultiplication() {
    C = make_shared<DenseMatrix>(B->size());

    for (int i = 0; i < pDivC(); ++i) {
        *C += A * B;
        shiftMatrixA(); // TODO maybe do not perform last shift
    }

//    for (int i = 0; i < world.size(); ++i) {
//        world.barrier();
//        if (world.rank() == i) {
//            cout << world.rank() << ": B fragment:" << endl << *B << endl;
//            cout << world.rank() << ": After multiplication:" << endl << *C << endl;
//        }
//    }
}

void ColAAlgorithm::step7_setResultAsNewBMatrix() {
    B = C;
    *C = 0.0;
}

void ColAAlgorithm::step9_printResultMatrix() {
    if (isCoordinator()) {
        vector<shared_ptr<DenseMatrixFragment>> submatrices((size_t) p());
        boost::mpi::gather(world, C, submatrices, 0);

        cout << "Gathered matrices: " << submatrices.size() << endl;

        auto result = DenseMatrix::mergeCols(submatrices);
        cout << *result << endl;
    } else {
        boost::mpi::gather(world, C, static_cast<shared_ptr<DenseMatrixFragment>*>(0), 0);
    }
}

void ColAAlgorithm::shiftMatrixA() {
    shared_ptr<SparseMatrix> newA(new SparseMatrix());

    boost::mpi::request requests[2];

    const int nextPeer     = (j() + 1) % p();
    const int previousPeer = (j() + p() - 1) % p();
    requests[0] = world.isend(    nextPeer, SHIFT_A, *A);
    requests[1] = world.irecv(previousPeer, SHIFT_A, *newA);

    boost::mpi::wait_all(requests, requests + 2);

    // Old A is automatically deleted by shared_ptr
    A = newA;
}


void ColAAlgorithm::step8_countAndPrintGe(double geElement) {
    int localCount = C->countGreaterOrEqual(geElement);
    int globalCount = 0;

    boost::mpi::reduce(world, localCount, globalCount, std::plus<int>(), 0);

    if (isCoordinator()) {
        cout << "GE count: " << globalCount;
    }
}