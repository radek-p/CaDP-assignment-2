//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "ColAAlgorithm.h"
#include "MatrixOperations.h"

void ColAAlgorithm::step6_performSingleMultiplication() {
    for (int i = 0; i < pDivC(); ++i) {
        *C += A * B;
        shiftMatrixA(); // TODO maybe do not perform last shift
    }
}

void ColAAlgorithm::step4_redistributeMatrixA() {
    vector<SparseMatrix> fragments;

    // TODO Send and receive matrices for merge

    A = SparseMatrixFragment::mergeCols(fragments);
}