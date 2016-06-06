//
// Created by Radosław Piórkowski on 05.06.2016.
//

#include "DenseMatrixFragment.h"

using namespace std;


const DenseMatrixFragment::DenseMatrixFragmentDescriptor &DenseMatrixFragment::size() const {
    return _size;
}

template<class Archive>
void DenseMatrixFragment::serialize(Archive & ar, const unsigned int version) {
    ar & _size;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeRows(
        const std::vector<DenseMatrixFragment> &fragmentsToConcat) {
    auto res = shared_ptr<DenseMatrixFragment>(new DenseMatrixFragment());

    return res;
}

std::shared_ptr<DenseMatrixFragment> DenseMatrixFragment::mergeCols(
        const std::vector<DenseMatrixFragment> &fragmentsToConcat) {
    auto res = shared_ptr<DenseMatrixFragment>(new DenseMatrixFragment());

    return res;
}


DenseMatrixFragment &DenseMatrixFragment::operator+=(DeferredSparseDenseMultiplication AtB) {
    AtB.addTo(*this);
    return *this;
}

DenseMatrixFragment &DenseMatrixFragment::operator=(DeferredSparseDenseMultiplication AtB) {
    AtB.writeTo(*this);
    return *this;
}

