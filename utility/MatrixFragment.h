//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_MATRIXFRAGMENT_H
#define MATRIXMUL_MATRIXFRAGMENT_H

#include <cstdint>
#include <memory>
#include <vector>

class MatrixFragment;
class SparseMatrixFragment;
class DenseMatrixFragment;
class DeferredSparseDenseMultiplication;
typedef MatrixFragment Matrix;

class MatrixFragment {
public:
    typedef double element_t;

    class MatrixFragmentDescriptor {
        friend class MatrixFragment;
        friend class SparseMatrixFragment;
        friend class DenseMatrixFragment;

    public:
        int matrixHeight() const;
        int matrixWidth() const;
        int pRow() const;
        int pCol() const;
        int kRow() const;
        int kCol() const;
        int fragmentHeight() const;
        int fragmentWidth() const;

        void fragmentHeight(int fragmentHeight);
        void fragmentWidth(int fragmentWidth);
        void pCol(int pCol);
        void pRow(int pRow);
        void kRow(int pRow);
        void kCol(int pCol);
        void matrixWidth(int matrixWidth);
        void matrixHeight(int matrixHeight);

        int getDataSize() const;

    private:
        // Size of the whole matrix
        int matrixHeight_, matrixWidth_;

        // Position of the top left corner of the fragment
        int pRow_, pCol_;

        // Width and height of the matrix fragment
        int fragmentHeight_, fragmentWidth_;
    };
};

#endif //MATRIXMUL_MATRIXFRAGMENT_H
