//
// Created by Radosław Piórkowski on 06.06.2016.
//

#include "MatrixOperations.h"


void DeferredSparseDenseMultiplication::addTo(DenseMatrixFragment &C) {
    exec(C, false);
}

void DeferredSparseDenseMultiplication::writeTo(DenseMatrixFragment &C) {
    exec(C, true);
}

DeferredSparseDenseMultiplication operator*(std::shared_ptr<SparseMatrixFragment> A,
                                           std::shared_ptr<DenseMatrixFragment> B) {
    return DeferredSparseDenseMultiplication(A, B);
}

DeferredSparseDenseMultiplication::DeferredSparseDenseMultiplication(
        std::shared_ptr<SparseMatrixFragment> A,
        std::shared_ptr<DenseMatrixFragment> B) : A(A), B(B) { }

void DeferredSparseDenseMultiplication::exec(DenseMatrixFragment &C, bool override) {

    if (override)
        C = 0.0;

    // TODO Perform multiplication
}



