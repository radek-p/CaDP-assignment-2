//
// Created by Radosław Piórkowski on 05.06.2016.
//

#ifndef MATRIXMUL_DENSEMATRIX_H
#define MATRIXMUL_DENSEMATRIX_H

#include "MatrixFragment.h"
#include "MatrixOperations.h"

#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/base_object.hpp>


typedef DenseMatrixFragment DenseMatrix;

class DenseMatrixFragment : public Matrix {
    friend class DeferredSparseDenseMultiplication;
    friend std::ostream& operator<< (std::ostream& stream, const DenseMatrix& matrix);
public:
    DenseMatrixFragment() { }

    DenseMatrixFragment(const MatrixFragmentDescriptor &size);

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

    DenseMatrixFragment &operator=(
            element_t initialValue
    );

    int countGreaterOrEqual(element_t threshold) const;

private:
    // Information about matrix dimensions
    MatrixFragmentDescriptor size_;

    std::vector<double> data_;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int) {
        ar & boost::serialization::base_object<MatrixFragment>(*this);
        ar & size_ & data_;
    };
};

std::ostream& operator<< (std::ostream& stream, const DenseMatrix& matrix);

#endif //MATRIXMUL_DENSEMATRIX_H
