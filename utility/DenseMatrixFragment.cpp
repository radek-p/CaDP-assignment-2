//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "DenseMatrixFragment.h"

#include <cassert>

using namespace std;


const DenseMatrixFragment::MatrixFragmentDescriptor &DenseMatrixFragment::size() const {
    return size_;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeRows(
        const std::vector<shared_ptr<DenseMatrixFragment>> &fragmentsToConcat) {

    auto res = createMatrixForMerge(fragmentsToConcat);

    for (const auto &item : fragmentsToConcat) {
        for (int i = item->size().pRow(); i < item->size().kRow(); ++i) {
            for (int j = item->size().pCol(); j < item->size().kCol(); ++j) {
                res->at(i, j) = item->at(i, j); // TODO Maybe improve efficency
            }
        }
    }

    return res;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeCols(
        const std::vector<shared_ptr<DenseMatrixFragment>> &fragmentsToConcat) {

    // In current implementation the same as merge of rows.
    return mergeRows(fragmentsToConcat);
}

DenseMatrixFragment::DenseMatrixFragment(const MatrixFragmentDescriptor &size) :
        size_(size), data_((size_t) size.getDataSize())
{ }

MatrixFragment::element_t &DenseMatrixFragment::at(int i, int j) {
    return data_[(i - size().pRow()) * size().fragmentWidth() + (j - size().pCol())];
}

const MatrixFragment::element_t &DenseMatrixFragment::at(int i, int j) const {
    return data_[(i - size().pRow()) * size().fragmentWidth() + (j - size().pCol())];
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::createMatrixForMerge(
        const std::vector<shared_ptr<DenseMatrixFragment>> &fragmentsToConcat) {

    assert(fragmentsToConcat.size() > 0);

    const auto & size = fragmentsToConcat[0]->size();

    int minPCol = size.pCol();
    int minPRow = size.pRow();
    int maxKCol = size.kCol();
    int maxKRow = size.kRow();

    for (const auto &item : fragmentsToConcat) {
        minPCol = min(minPCol, item->size().pCol());
        minPRow = min(minPRow, item->size().pRow());
        maxKCol = max(maxKCol, item->size().kCol());
        maxKRow = max(maxKRow, item->size().kRow());
    }

    MatrixFragmentDescriptor dscr = size;
    dscr.pCol(minPCol);
    dscr.pRow(minPRow);
    dscr.kCol(maxKCol);
    dscr.kRow(maxKRow);

    return shared_ptr<DenseMatrixFragment>(new DenseMatrixFragment(dscr));
}

DenseMatrixFragment &DenseMatrixFragment::operator+=(DeferredSparseDenseMultiplication AtB) {
    AtB.addTo(*this);
    return *this;
}

DenseMatrixFragment &DenseMatrixFragment::operator=(DeferredSparseDenseMultiplication AtB) {
    AtB.writeTo(*this);
    return *this;
}

DenseMatrixFragment &DenseMatrixFragment::operator=(element_t initialValue) {
    std::fill(data_.begin(), data_.end(), initialValue);
    return *this;
}

int DenseMatrixFragment::countGreaterOrEqual(element_t threshold) const {
    int res = 0;
    for (size_t i = 0; i < data_.size(); ++i)
        if (data_[i] >= threshold)
            ++res;
    return res;
}

std::ostream &operator<<(std::ostream &stream, const DenseMatrix &matrix) {
    stream << matrix.size().matrixHeight() << "\t" << matrix.size().matrixWidth() << endl;
//    stream << "matrix[" << matrix.size().matrixWidth() << "x" << matrix.size().matrixHeight()
//    << "] with #entries = " << matrix.data_.size() << ", pCol = " << matrix.size().pCol()
//            << ", pRow = " << matrix.size().pRow() << ", kCol = " << matrix.size().kCol()
//            << ", kRow = " << matrix.size().kRow() << endl;

    for (int i = 0; i < matrix.size().pRow(); ++i) {
        for (int j = 0; j < matrix.size().matrixWidth(); ++j)
            stream << "\t" << "_";
        stream << "\n";
    }

    for (int i = matrix.size().pRow(); i < matrix.size().matrixHeight(); ++i) {
        for (int j = 0; j < matrix.size().pCol(); ++j)
            stream << "\t" << "_";

        for (int j = matrix.size().pCol(); j < matrix.size().kCol(); ++j)
            stream << "\t" << matrix.at(i, j);

        for (int j = matrix.size().kCol(); j < matrix.size().matrixWidth(); ++j)
            stream << "\t" << "_";

        stream << "\n";
    }

    for (int i = matrix.size().kRow(); i < matrix.size().matrixHeight(); ++i) {
        for (int j = 0; j < matrix.size().matrixWidth(); ++j)
            stream << "\t" << "_";
        stream << "\n";
    }

    return stream;
}














