//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "SparseMatrixFragment.h"

#include <iostream>
#include <fstream>
#include <cassert>

using namespace std;


std::ostream &operator<<(std::ostream &stream, const SparseMatrix &matrix) {

    // TODO delete
    stream << "matrix[" << matrix.size().matrixWidth() << "x" << matrix.size().matrixHeight()
           << "] with #entries = " << matrix.entries.size() << ", nnz = " << matrix.size().nnz() << endl;

    int rowBeginIncl = matrix.rowIntervals[0], rowEndExcl;
    for (int rowIdx = 0; rowIdx < matrix.size().matrixHeight(); ++rowIdx) {
        rowEndExcl = matrix.rowIntervals[rowIdx+1];

        int colIdx = 0;
        for (int itemIdx = rowBeginIncl; itemIdx < rowEndExcl; ++itemIdx) {
            while (colIdx < matrix.columns[itemIdx]) {
                stream << "\t" << 0.0;
                ++colIdx;
            }
            stream << "\t" << matrix.entries[itemIdx];
            ++colIdx;
        }
        while (colIdx < matrix.size().matrixWidth()) {
            stream << "\t" << 0.0;
            ++colIdx;
        }
        stream << "\n";
        rowBeginIncl = rowEndExcl;
    }

    return stream;
}

const SparseMatrixFragment::SparseMatrixFragmentDescriptor &SparseMatrixFragment::size() const {
    return size_;
}

SparseMatrixFragment::SparseMatrixFragment(const std::string &fileName) {
    loadFromFile(fileName);
}

void SparseMatrixFragment::loadFromFilePrivate(const std::string &fileName) {
    ifstream matrixSource;
    matrixSource.open(fileName);
    matrixSource >> size_.matrixWidth_
    >> size_.matrixHeight_
    >> size_.nnz_
    >> size_.maxItemsPerRow_;

    size_.fragmentHeight_ = size_.matrixHeight_;
    size_.fragmentWidth_  = size_.matrixWidth_;

    size_.pCol_ = 0;
    size_.pRow_ = 0;

    entries.resize((size_t) size().nnz());
    rowIntervals.resize((size_t) (size().matrixHeight() + 1));
    columns.resize((size_t) size().nnz());
    for (int i = 0; i < size().nnz(); ++i) {
        matrixSource >> entries[i];
    }

    for (int i = 0; i <= size().matrixHeight(); ++i) {
        matrixSource >> rowIntervals[i];
    }

    for (int i = 0; i < size().nnz(); ++i) {
        matrixSource >> columns[i];
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

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::loadFromFile(const std::string &fileName) {
    auto res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment());
    res->loadFromFilePrivate(fileName);

    return res;
}

void SparseMatrixFragment::splitIntoColumnGroups(
        const std::vector<int> &rowDivision,
        vector<SparseMatrixFragment> &res) const {

    assert(rowIntervals.size() > 0);
    assert(!size().isFragment());
    assert(rowDivision[rowDivision.size() - 1] == size().matrixWidth());

    res.resize(rowDivision.size() - 1);

    int numBlocks = static_cast<int>(rowDivision.size() - 1);
    for (int i = 0; i < numBlocks; ++i) {
        res[i] = SparseMatrixFragment(size());
        res[i].size_.pCol(rowDivision[i]);
        res[i].size_.kCol(rowDivision[i+1]);
        res[i].rowIntervals.push_back(0);
        res[i].size_.nnz_ = 0;
    }

    int rowBeginIncl = rowIntervals[0], rowEndExcl;
    // Iterate by rows.
    for (int i = 0; i < size().matrixHeight(); ++i) {
        rowEndExcl = rowIntervals[i+1];

        // Read one row and copy its elements to appropriate submatrices.
        int dividerIdx = 0;
        for (int elEntryIdx = rowBeginIncl; elEntryIdx < rowEndExcl; ++elEntryIdx) {
            // Current element is in entries[elEntryIdx]; columns[elEntryIdx];
            while (rowDivision[dividerIdx+1] <= columns[elEntryIdx])
                ++dividerIdx;

            res[dividerIdx].entries.push_back(entries[elEntryIdx]);
            res[dividerIdx].columns.push_back(columns[elEntryIdx]);
            res[dividerIdx].size_.nnz_++;
        }

        // Finalize each row entry in each submatrix
        for (auto &resItem : res) {
            resItem.rowIntervals.push_back(static_cast<int>(resItem.entries.size()));
        }
        rowBeginIncl = rowEndExcl;
    }
}

SparseMatrixFragment::SparseMatrixFragment(const SparseMatrixFragmentDescriptor &descriptor) {
    size_ = descriptor;
}

void SparseMatrixFragment::splitIntoRowGroups(const vector<int> &columnDivision, vector<SparseMatrixFragment> &res) const {
    assert(rowIntervals.size() == size().matrixHeight());
    assert(!size().isFragment());
    assert(columnDivision[columnDivision.size() - 1] == size().matrixHeight());

    res.resize(columnDivision.size() - 1);

//    int numBlocks = static_cast<int>(columnDivision.size() - 1);
//    for (int i = 0; i < numBlocks; ++i) {
//        res[i] = SparseMatrixFragment(size());
//        res[i].size_.pRow(rowDivision[i]);
//        res[i].size_.kRow(rowDivision[i+1]);
//        res[i].rowIntervals.push_back(0);
//        res[i].size_.nnz_ = 0;
//    }

//    int rowBeginIncl = rowIntervals[0], rowEndExcl;
//    // Iterate by rows.
//    for (int i = 0; i < size().matrixHeight(); ++i) {
//        rowEndExcl = rowIntervals[i+1];
//
//        // Read one row and copy its elements to appropriate submatrices.
//        int dividerIdx = 0;
//        for (int elEntryIdx = rowBeginIncl; elEntryIdx < rowEndExcl; ++elEntryIdx) {
//            // Current element is in entries[elEntryIdx]; columns[elEntryIdx];
//            while (rowDivision[dividerIdx+1] <= columns[elEntryIdx])
//                ++dividerIdx;
//
//            res[dividerIdx].entries.push_back(entries[elEntryIdx]);
//            res[dividerIdx].columns.push_back(columns[elEntryIdx]);
//            res[dividerIdx].size_.nnz_++;
//        }
//
//        // Finalize each row entry in each submatrix
//        for (auto &resItem : res) {
//            resItem.rowIntervals.push_back(static_cast<int>(resItem.entries.size()));
//        }
//        rowBeginIncl = rowEndExcl;
//    }
}


int SparseMatrixFragment::SparseMatrixFragmentDescriptor::nnz() const {
    return nnz_;
}

int SparseMatrixFragment::SparseMatrixFragmentDescriptor::maxItemsPerRow() const {
    return maxItemsPerRow_;
}


