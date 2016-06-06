//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_SPARSEMATRIX_H
#define MATRIXMUL_SPARSEMATRIX_H

#include <vector>
#include <string>
#include <boost/serialization/vector.hpp>

#include "MatrixFragment.h"
#include "MatrixOperations.h"

class SparseMatrixFragment;
typedef SparseMatrixFragment SparseMatrix;

class SparseMatrixFragment : public MatrixFragment {
public:
    class SparseMatrixFragmentDescriptor : public MatrixFragmentDescriptor {
        friend class SparseMatrixFragment;
    private:
    public:
        int nnz() const;
        int maxItemsPerRow() const;

    private:
        int maxItemsPerRow_, nnz_;
    };

    // Information about matrix dimensions
    SparseMatrixFragmentDescriptor size_;
    const SparseMatrixFragmentDescriptor &size() const;

    SparseMatrixFragment(const std::string &fileName);

    std::shared_ptr<SparseMatrixFragment> rowSubmatrix(int firstRowIncl, int lastRowExcl) const;
    std::shared_ptr<SparseMatrixFragment> columnSubmatrix(int firstColIncl, int lastColExcl) const;

    static std::shared_ptr<SparseMatrixFragment> mergeRows(const std::vector<SparseMatrixFragment> &fragmentsToConcat);
    static std::shared_ptr<SparseMatrixFragment> mergeCols(const std::vector<SparseMatrixFragment> &fragmentsToConcat);

    friend std::ostream& operator<< (std::ostream& stream, const SparseMatrix& matrix);

private:
    std::vector<element_t> entries;
    std::vector<int> columns;
    std::vector<int> rowIntervals;

    // Constructor used by internal methods that
    // have to perform custom initialisation
    SparseMatrixFragment() {}

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version);

    void loadFromFile(const std::string &fileName);
};


#endif //MATRIXMUL_SPARSEMATRIX_H
