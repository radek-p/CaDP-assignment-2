//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "InnerABCAlgorithm.h"


void InnerABCAlgorithm::step4_redistributeMatrixA() {
    vector<SparseMatrix> fragments;

    // TODO Send and receive matrices for merge

    A = SparseMatrixFragment::mergeRows(fragments);

    for (int i = 0; i < q() * l(); ++i) {
        shiftMatrixA();
    }
}

void InnerABCAlgorithm::step5_redistributeMatrixB() {
    vector<DenseMatrix> fragments;

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