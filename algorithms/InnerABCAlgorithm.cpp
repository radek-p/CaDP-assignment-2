//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "InnerABCAlgorithm.h"
#include <boost/mpi/collectives.hpp>
#include <boost/serialization/shared_ptr.hpp>


InnerABCAlgorithm::InnerABCAlgorithm(int _c) :
        GenericMultiplicationAlgorithm(_c),
        replicationGroup(world.split(j() / c(), j() % c())),
        orthogonalGroup(world.split(j() % c(), j() / c()))
{
    cout << "InnerABC algorithm process"
        << "(" << replicationGroup.size() << ", " << orthogonalGroup.size()
        << ") <- (" << replicationGroup.rank() << ", " << orthogonalGroup.rank() << ")" << endl;
}

bool InnerABCAlgorithm::splitAInRowGroups() {
    // InnerABC algorithm splits matrix A into row groups.
    return true;
}

void InnerABCAlgorithm::step4_redistributeMatrixA() {
    vector<shared_ptr<SparseMatrix>> fragments;

    boost::mpi::all_gather(replicationGroup, A, fragments);
    A = SparseMatrixFragment::mergeRows(fragments);

    world.barrier();
    if (world.rank() == 6) {
        cout << "replication group leader with world rank: "
        << world.rank() << " gathered A matrix" << *A << endl;
    }

    for (int i = 0; i < q() * l(); ++i) {
        shiftMatrixA();
    }
}

void InnerABCAlgorithm::step5_redistributeMatrixB() {
    vector<shared_ptr<DenseMatrix>> fragments;

    boost::mpi::all_gather(replicationGroup, B, fragments);
    B = DenseMatrixFragment::mergeCols(fragments);

    world.barrier();
    if (world.rank() == 0) {
        cout << "replication group leader with world rank: "
        << world.rank() << " gathered B matrix" << *B << endl;
    }
}

void InnerABCAlgorithm::step6_performSingleMultiplication() {

    C.resize(0);
    for (int i = 0; i < q(); ++i) {
        Matrix::MatrixFragmentDescriptor descriptor = B->size();
        descriptor.pRow(A->size().pRow());
        descriptor.kRow(A->size().kRow());
        C.push_back(make_shared<DenseMatrixFragment>(descriptor));

        *C[i] = A * B;

        shiftMatrixA();
    }
}

void InnerABCAlgorithm::step7_setResultAsNewBMatrix() {

    vector<vector<shared_ptr<DenseMatrixFragment>>> partsOfMatrixC;

    boost::mpi::all_gather(replicationGroup, C, partsOfMatrixC);

    vector<shared_ptr<DenseMatrixFragment>> partsOfMatrixCFlattened;
    for (const auto & vec : partsOfMatrixC) {
        for (const auto & ptr : vec) {
            partsOfMatrixCFlattened.push_back(ptr);
        }
    }

    B = DenseMatrix::mergeRows(partsOfMatrixCFlattened);
//    cout << "After gather we have partial result B: " << *B << endl;
    C.resize(0);
}

void InnerABCAlgorithm::shiftMatrixA() {
    shared_ptr<SparseMatrix> newA(new SparseMatrix());

    boost::mpi::request requests[2];

    const int nextPeer     = (orthogonalGroup.rank() + 1) % orthogonalGroup.size();
    const int previousPeer = (orthogonalGroup.rank() + orthogonalGroup.size() - 1) % orthogonalGroup.size();
    requests[0] = orthogonalGroup.isend(previousPeer, SHIFT_A, *A   );
    requests[1] = orthogonalGroup.irecv(    nextPeer, SHIFT_A, *newA);

    boost::mpi::wait_all(requests, requests + 2);

    // Old A is automatically deleted by shared_ptr
    A = newA;
}

void InnerABCAlgorithm::step9_printResultMatrix() {
    vector<vector<shared_ptr<DenseMatrixFragment>>> partsOfMatrixC;

    boost::mpi::gather(replicationGroup, C, partsOfMatrixC, 0);

    if (replicationGroup.rank() == 0) {
        vector<shared_ptr<DenseMatrixFragment>> partsOfMatrixCFlattened;
        vector<shared_ptr<DenseMatrixFragment>> partsOfResult;
        for (const auto & vec : partsOfMatrixC) {
            for (const auto & ptr : vec) {
                partsOfMatrixCFlattened.push_back(ptr);
            }
        }

        B = DenseMatrix::mergeRows(partsOfMatrixCFlattened);

        boost::mpi::gather(orthogonalGroup, B, partsOfResult, 0);

        if (orthogonalGroup.rank() == 0) {
            B = DenseMatrix::mergeCols(partsOfResult);
            cout << "After gather we have final result B: " << *B << endl;
        }
    }
}

void InnerABCAlgorithm::step8_countAndPrintGe(double geElement) {
    int localCount = 0;
    for (const auto &item : C)
        localCount += item->countGreaterOrEqual(geElement);

    int globalCount = 0;

    boost::mpi::reduce(world, localCount, globalCount, std::plus<int>(), 0);

    if (isCoordinator()) {
        cout << "GE count: " << globalCount;
    }
}