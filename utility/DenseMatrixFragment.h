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
    class DenseMatrixFragmentDescriptor : public MatrixFragmentDescriptor {

    };

    // Information about matrix dimensions
    DenseMatrixFragmentDescriptor _size;
    const DenseMatrixFragmentDescriptor &size() const;

    static std::shared_ptr<DenseMatrixFragment> mergeRows(const std::vector<DenseMatrixFragment> &fragmentsToConcat);
    static std::shared_ptr<DenseMatrixFragment> mergeCols(const std::vector<DenseMatrixFragment> &fragmentsToConcat);

    DenseMatrixFragment &operator+=(
            DeferredSparseDenseMultiplication AtB
    );

    DenseMatrixFragment &operator=(
            DeferredSparseDenseMultiplication AtB
    );

private:
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);
};


#endif //MATRIXMUL_DENSEMATRIX_H
