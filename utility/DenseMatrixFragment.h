//
// Created by Radosław Piórkowski on 05.06.2016.
//

#ifndef MATRIXMUL_DENSEMATRIX_H
#define MATRIXMUL_DENSEMATRIX_H

#include "MatrixFragment.h"
#include "MatrixOperations.h"

#include <vector>


class DenseMatrixFragment;
typedef DenseMatrixFragment DenseMatrix;

class DenseMatrixFragment : public Matrix {
public:
    DenseMatrixFragment(const MatrixFragmentDescriptor &size);

    // Information about matrix dimensions
    MatrixFragmentDescriptor _size;
    const MatrixFragmentDescriptor &size() const;

    element_t &at(int i, int j);
    const element_t &at(int i, int j) const;

    static std::shared_ptr<DenseMatrixFragment> createMatrixForMerge(const std::vector<DenseMatrixFragment> &fragmentsToConcat);
    static std::shared_ptr<DenseMatrixFragment> mergeRows(const std::vector<DenseMatrixFragment> &fragmentsToConcat);
    static std::shared_ptr<DenseMatrixFragment> mergeCols(const std::vector<DenseMatrixFragment> &fragmentsToConcat);

    DenseMatrixFragment &operator+=(
            DeferredSparseDenseMultiplication AtB
    );

    DenseMatrixFragment &operator=(
            DeferredSparseDenseMultiplication AtB
    );

private:
    std::vector<double> data_;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
};


#endif //MATRIXMUL_DENSEMATRIX_H
