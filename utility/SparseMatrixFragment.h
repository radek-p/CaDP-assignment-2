//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_SPARSEMATRIX_H
#define MATRIXMUL_SPARSEMATRIX_H

#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include <boost/serialization/base_object.hpp>

#include <boost/mpi/communicator.hpp>

#include "MatrixFragment.h"
#include "MatrixOperations.h"

typedef SparseMatrixFragment SparseMatrix;

class SparseMatrixFragment : public MatrixFragment {
    boost::mpi::communicator world;
    friend class DeferredSparseDenseMultiplication;
public:
    class SparseMatrixFragmentDescriptor : public MatrixFragmentDescriptor {
        friend class SparseMatrixFragment;
    private:
    public:
        int nnz() const;
        int maxItemsPerRow() const;

    private:
        int maxItemsPerRow_, nnz_;

        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & boost::serialization::base_object<MatrixFragmentDescriptor>(*this);
            ar & maxItemsPerRow_ & nnz_;
        }
    };

    // Information about matrix dimensions
    const SparseMatrixFragmentDescriptor &size() const;

    // TODO Maybe remove
    SparseMatrixFragment(const std::string &fileName);

    SparseMatrixFragment(const SparseMatrixFragmentDescriptor &descriptor);

    // These methods return submatrices that only contain rows / columns in given interval
    void splitIntoGroups(const std::vector<int> rowDivision, std::vector<std::shared_ptr<SparseMatrixFragment>> &res, bool groupRows) const;
    void splitIntoColumnGroups(const std::vector<int> &rowDivision, std::vector<std::shared_ptr<SparseMatrixFragment>> &res) const;
    void splitIntoRowGroups(const std::vector<int> &columnDivision, std::vector<std::shared_ptr<SparseMatrixFragment>> &res) const;

    static void setMergeDimensions(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat, SparseMatrixFragmentDescriptor &descr);
    static std::shared_ptr<SparseMatrixFragment> mergeRows(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat);
    static std::shared_ptr<SparseMatrixFragment> mergeCols(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat);

    friend std::ostream& operator<< (std::ostream& stream, const SparseMatrix& matrix);

    static std::shared_ptr<SparseMatrixFragment> loadFromFile(const std::string &fileName);

    virtual ~SparseMatrixFragment() { /*std::cout << world.rank() << ": Sparse matrix was deleted!" << std::endl; */ } // TODO delete that later

    // Constructor used by internal methods that
    // have to perform custom initialisation
    SparseMatrixFragment() {}

private:
    SparseMatrixFragmentDescriptor size_;
    std::vector<element_t> entries;
    std::vector<int> columns;
    std::vector<int> rowIntervals;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<MatrixFragment>(*this);
        ar & size_ & entries & rowIntervals & columns;
    }

    void loadFromFilePrivate(const std::string &fileName);
};

BOOST_IS_MPI_DATATYPE(SparseMatrixFragment::SparseMatrixFragmentDescriptor)

std::ostream& operator<< (std::ostream& stream, const SparseMatrix& matrix);

#endif //MATRIXMUL_SPARSEMATRIX_H
