//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "MatrixFragment.h"

#include <cstdlib>

//MatrixFragment::MatrixFragment(size_t bufferSize) {
////    __data = std::make_sharedmalloc(bufferSize);
//}

long send() {
    return 0;
}

int MatrixFragment::MatrixFragmentDescriptor::matrixHeight() const { return matrixHeight_; }

int MatrixFragment::MatrixFragmentDescriptor::matrixWidth() const { return matrixWidth_; }

int MatrixFragment::MatrixFragmentDescriptor::pRow() const { return pRow_; }

int MatrixFragment::MatrixFragmentDescriptor::pCol() const { return pCol_; }

int MatrixFragment::MatrixFragmentDescriptor::fragmentHeight() const { return fragmentHeight_; }

int MatrixFragment::MatrixFragmentDescriptor::fragmentWidth() const { return fragmentWidth_; }

void MatrixFragment::MatrixFragmentDescriptor::fragmentHeight(int fragmentHeight) { fragmentHeight_ = fragmentHeight; }

void MatrixFragment::MatrixFragmentDescriptor::fragmentWidth(int fragmentWidth) { fragmentWidth_ = fragmentWidth; }

void MatrixFragment::MatrixFragmentDescriptor::pCol(int pCol) { pCol_ = pCol; }

void MatrixFragment::MatrixFragmentDescriptor::pRow(int pRow) { pRow_ = pRow; }

void MatrixFragment::MatrixFragmentDescriptor::matrixWidth(int matrixWidth) { matrixWidth_ = matrixWidth; }

void MatrixFragment::MatrixFragmentDescriptor::matrixHeight(int matrixHeight) { matrixHeight_ = matrixHeight; }

int MatrixFragment::MatrixFragmentDescriptor::kRow() const { return pRow_ + fragmentHeight_; }

int MatrixFragment::MatrixFragmentDescriptor::kCol() const { return pCol_ + fragmentWidth_; }

void MatrixFragment::MatrixFragmentDescriptor::kRow(int kRow) { fragmentHeight_ = kRow - pRow_; }

void MatrixFragment::MatrixFragmentDescriptor::kCol(int kCol) { fragmentWidth_ = kCol - pCol_; }

int MatrixFragment::MatrixFragmentDescriptor::getDataSize() const {
    return fragmentWidth() * fragmentHeight();
}









