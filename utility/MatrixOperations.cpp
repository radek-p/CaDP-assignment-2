//
// Created by Radosław Piórkowski on 06.06.2016.
//

#include "MatrixOperations.h"


void DeferredSparseDenseMultiplication::addTo(DenseMatrixFragment &C) {

}

void DeferredSparseDenseMultiplication::writeTo(DenseMatrixFragment &C) {

}

DeferredSparseDenseMultiplication operator*(std::shared_ptr<SparseMatrixFragment> A,
                                           std::shared_ptr<DenseMatrixFragment> B) {
    return DeferredSparseDenseMultiplication(A, B);
}

DeferredSparseDenseMultiplication::DeferredSparseDenseMultiplication(
        std::shared_ptr<SparseMatrixFragment> A,
        std::shared_ptr<DenseMatrixFragment> B) : A(A), B(B) { }