//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_MATRIXOPERATIONS_H
#define MATRIXMUL_MATRIXOPERATIONS_H

#include "SparseMatrixFragment.h"
#include "DenseMatrixFragment.h"

class DeferredSparseDenseMultiplication {
public:
    DeferredSparseDenseMultiplication(
                std::shared_ptr<SparseMatrixFragment> A,
                std::shared_ptr<DenseMatrixFragment> B);

    void addTo(DenseMatrixFragment &C);
    void writeTo(DenseMatrixFragment &C);

private:
    std::shared_ptr<SparseMatrixFragment> A;
    std::shared_ptr<DenseMatrixFragment> B;

    void exec(DenseMatrixFragment &C, bool override = false);
};

DeferredSparseDenseMultiplication operator*(
        std::shared_ptr<SparseMatrixFragment> A,
        std::shared_ptr<DenseMatrixFragment> B);

#endif //MATRIXMUL_MATRIXOPERATIONS_H
