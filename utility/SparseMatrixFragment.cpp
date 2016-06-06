//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "SparseMatrixFragment.h"

#include <iostream>
#include <fstream>

using namespace std;

//SparseMatrix::SparseMatrix(std::string sourceFile, bool storeInColumnMajorOrder) {
//    loadFromFile(sourceFile, storeInColumnMajorOrder);
//}

//double SparseMatrix::at(int i, int j) const {
//    if (!rowMajor)
//        swap(i, j);
//
//    map<int, double>::const_iterator iterator    = data[i].find(j);
//    map<int, double>::const_iterator endIterator = data[i].end();
//
//    return (iterator == endIterator) ? 0.0 : iterator->second;
//}

//std::ostream &operator<<(std::ostream &stream, const SparseMatrix &matrix) {
//    for (int rowIdx = 0; rowIdx < matrix.height; ++rowIdx) {
//        if (matrix.width > 0) {
//            stream << matrix.at(rowIdx, 0);
//        }
//        for (int colIdx = 1; colIdx < matrix.width; ++colIdx) {
//            stream << "\t" << matrix.at(rowIdx, colIdx);
//        }
//        stream << "\n";
//    }
//    return stream;
//}

//SparseMatrixFragment::SparseMatrixFragment(string sourceFile) {
//
//}


//void SparseMatrix::loadFromFile(std::string sourceFile, bool storeInColumnMajorOrder) {
//    ifstream matrixSource;
//    matrixSource.open(sourceFile);
//    matrixSource >> width >> height >> nnz >> maxItemsPerRow;
//
//    vector<double> nnzEntries;
//
//    double nnzEntry;
//    for (int i = 0; i < nnz; ++i) {
//        matrixSource >> nnzEntry;
//        nnzEntries.push_back(nnzEntry);
//    }
//    vector<int> rowIntervals;
//
//    int tmp2;
//    for (int i = 0; i <= height; ++i) {
//        matrixSource >> tmp2;
//        rowIntervals.push_back(tmp2);
//    }
//
//    int tmpColIdx;
//    if (storeInColumnMajorOrder) {
//        rowMajor = false;
//        int nnzEntryIdx = 0;
//        for (int colIdx = 0; colIdx < width; ++colIdx) {
//            data.push_back(map<int, double>());
//        }
//
//        for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
//            const int size = rowIntervals[rowIdx + 1] - rowIntervals[rowIdx];
//            for (int rowElemIdx = 0; rowElemIdx < size; ++rowElemIdx) {
//                matrixSource >> tmpColIdx;
//                data[tmpColIdx][rowIdx] = nnzEntries[nnzEntryIdx];
//                nnzEntryIdx++;
//            }
//        }
//    } else {
//        rowMajor = true;
//        int nnzEntryIdx = 0;
//        for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
//            data.push_back(map<int, double>());
//            int size = rowIntervals[rowIdx + 1] - rowIntervals[rowIdx];
//            for (int rowElemIdx = 0; rowElemIdx < size; ++rowElemIdx) {
//                matrixSource >> tmpColIdx;
//                data[rowIdx][tmpColIdx] = nnzEntries[nnzEntryIdx];
//                nnzEntryIdx++;
//            }
//        }
//    }
//
//    matrixSource.close();
//}


std::ostream &operator<<(std::ostream &stream, const SparseMatrix &matrix) {
    return stream;
}

template<class Archive>
void SparseMatrixFragment::serialize(Archive &ar, const unsigned int version) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-value"
    ar & entries & rowIntervals & columns;
#pragma clang diagnostic pop
}

const SparseMatrixFragment::SparseMatrixFragmentDescriptor &SparseMatrixFragment::size() const {
    return size_;
}

SparseMatrixFragment::SparseMatrixFragment(const std::string &fileName) {
    loadFromFile(fileName);
}

void SparseMatrixFragment::loadFromFile(const std::string &fileName) {
    ifstream matrixSource;
    matrixSource.open(fileName);
    matrixSource >> size_.matrixWidth_
    >> size_.matrixHeight_
    >> size_.nnz_
    >> size_.maxItemsPerRow_;

    double nnzEntry;
    for (int i = 0; i < size().nnz(); ++i) {
        matrixSource >> nnzEntry;
        entries.push_back(nnzEntry);
    }

    int tmp2;
    for (int i = 0; i <= size().matrixHeight(); ++i) {
        matrixSource >> tmp2;
        rowIntervals.push_back(tmp2);
    }

    int tmpColIdx;
    for (int i = 0; i < size().nnz(); ++i) {
        matrixSource >> tmpColIdx;
        columns.push_back(tmpColIdx);
    }

    matrixSource.close();
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::mergeRows(const std::vector<SparseMatrixFragment> &fragmentsToConcat) {

    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);
    setMergeDimensions(fragmentsToConcat, res->size_);

    // TODO Implement sparse row merge

    return res;
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::mergeCols(const std::vector<SparseMatrixFragment> &fragmentsToConcat) {
    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);
    setMergeDimensions(fragmentsToConcat, res->size_);

    // TODO Implement sparse column merge

    return res;
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::rowSubmatrix(int firstRowIncl, int lastRowExcl) const {
    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);

    return res;
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::columnSubmatrix(int firstColIncl, int lastColExcl) const {
    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);

    return res;
}

void SparseMatrixFragment::setMergeDimensions(const std::vector<SparseMatrixFragment> &fragmentsToConcat,
                                              SparseMatrixFragmentDescriptor &descr) {

    // FIXME Maybe remove copy / paste code from DenseMatrixFragment
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

    descr.pCol(minPCol);
    descr.pRow(minPRow);
    descr.kCol(maxKCol);
    descr.kRow(maxKRow);
}


int SparseMatrixFragment::SparseMatrixFragmentDescriptor::nnz() const {
    return nnz_;
}

int SparseMatrixFragment::SparseMatrixFragmentDescriptor::maxItemsPerRow() const {
    return maxItemsPerRow_;
}


