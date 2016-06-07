//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "ColAAlgorithm.h"

#include <boost/mpi/collectives.hpp>
#include <memory>

#include "MatrixOperations.h"

using namespace std;

void ColAAlgorithm::step2_distributeMatrixA() {
    A = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment());

    if (isCoordinator()) {
        vector<SparseMatrixFragment> parts;
        auto division = prepareDivision(wholeA->size().matrixWidth(), p());
        wholeA->splitIntoColumnGroups(division, parts);

        boost::mpi::scatter(world, parts, *A, 0);
    } else {
        boost::mpi::scatter(world, static_cast<const SparseMatrixFragment*>(0), *A, 0);
    }

//    for (int i = 0; i < world.size(); ++i) {
//        if (world.rank() == i) {
//            cout << "scattered[" << world.rank() << "]" << endl;
//            cout << *A << endl;
//        }
//        world.barrier();
//    }

    if (isCoordinator())
        wholeA.reset(); // It deletes the wholeA matrix.
}

void ColAAlgorithm::step4_redistributeMatrixA() {
    vector<SparseMatrix> fragments;

    // TODO Send and receive matrices for merge

    A = SparseMatrixFragment::mergeCols(fragments);
}

void ColAAlgorithm::step6_performSingleMultiplication() {
    for (int i = 0; i < pDivC(); ++i) {
        *C += A * B;
        shiftMatrixA(); // TODO maybe do not perform last shift
    }
}

void ColAAlgorithm::step7_setResultAsNewBMatrix() {
    B = C;
    *C = 0.0;
}
