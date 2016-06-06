//
// Created by Radosław Piórkowski on 05.06.2016.
//

#include "DenseMatrixFragment.h"

using namespace std;


const DenseMatrixFragment::MatrixFragmentDescriptor &DenseMatrixFragment::size() const {
    return _size;
}

template<class Archive>
void DenseMatrixFragment::serialize(Archive & ar, const unsigned int version) {
    ar & _size;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeRows(
        const std::vector<DenseMatrixFragment> &fragmentsToConcat) {

    auto res = createMatrixForMerge(fragmentsToConcat);

    for (const auto &item : fragmentsToConcat) {
        for (int i = item.size().pRow(); i < item.size().kRow(); ++i) {
            for (int j = item.size().pCol(); j < item.size().kCol(); ++j) {
                res->at(i, j) = item.at(i, j); // TODO Maybe improve efficency
            }
        }
    }

    return res;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeCols(
        const std::vector<DenseMatrixFragment> &fragmentsToConcat) {

    // In current implementation the same as merge of rows.
    return mergeRows(fragmentsToConcat);
}

DenseMatrixFragment &DenseMatrixFragment::operator+=(DeferredSparseDenseMultiplication AtB) {
    AtB.addTo(*this);
    return *this;
}

DenseMatrixFragment &DenseMatrixFragment::operator=(DeferredSparseDenseMultiplication AtB) {
    AtB.writeTo(*this);
    return *this;
}

DenseMatrixFragment::DenseMatrixFragment(const MatrixFragmentDescriptor &size) :
    data_((size_t) size.getDataSize())
{ }

MatrixFragment::element_t &DenseMatrixFragment::at(int i, int j) {
    return data_[(i - size().pRow()) * size().fragmentWidth() + (j - size().pCol())];
}

const MatrixFragment::element_t &DenseMatrixFragment::at(int i, int j) const {
    return data_[(i - size().pRow()) * size().fragmentWidth() + (j - size().pCol())];
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::createMatrixForMerge(
        const std::vector<DenseMatrixFragment> &fragmentsToConcat) {

    assert(fragmentsToConcat.size() > 0);

    const auto & size = fragmentsToConcat[0].size();

    int minPCol = size.pCol();
    int minPRow = size.pRow();
    int maxKCol = size.kCol();
    int maxKRow = size.kRow();

    for (const auto &item : fragmentsToConcat) {
        minPCol = min(minPCol, item.size().pCol());
        minPRow = min(minPRow, item.size().pRow());
        maxKCol = max(maxKCol, item.size().kCol());
        maxKRow = max(maxKRow, item.size().kRow());
    }

    MatrixFragmentDescriptor dscr = size;
    dscr.pCol(minPCol);
    dscr.pRow(minPRow);
    dscr.kCol(maxKCol);
    dscr.kRow(maxKRow);

    return shared_ptr<DenseMatrixFragment>(new DenseMatrixFragment(dscr));
}










