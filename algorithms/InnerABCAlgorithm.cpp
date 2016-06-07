//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "InnerABCAlgorithm.h"


bool InnerABCAlgorithm::splitAInRowGroups() {
    // InnerABC algorithm splits matrix A into row groups.
    return true;
}

void InnerABCAlgorithm::step4_redistributeMatrixA() {
    vector<shared_ptr<SparseMatrix>> fragments;

    // TODO Send and receive matrices for merge

    A = SparseMatrixFragment::mergeRows(fragments);

    for (int i = 0; i < q() * l(); ++i) {
        shiftMatrixA();
    }
}

void InnerABCAlgorithm::step5_redistributeMatrixB() {
    vector<shared_ptr<DenseMatrix>> fragments;

    // TODO Send and receive matrices for merge

    B = DenseMatrixFragment::mergeCols(fragments);
}

void InnerABCAlgorithm::step6_performSingleMultiplication() {
    for (int i = 0; i < q(); ++i) {
        *C = A * B;
        shiftMatrixA(); // TODO Maybe not perform last shift
    }
}

void InnerABCAlgorithm::step7_setResultAsNewBMatrix() {
    // TODO Send C and set as B

    *C = 0;
}

void InnerABCAlgorithm::shiftMatrixA() {

}

void InnerABCAlgorithm::step9_printResultMatrix() {
    if (isCoordinator()) {
        cout << "The matrix should be printed" << endl;
    }
}