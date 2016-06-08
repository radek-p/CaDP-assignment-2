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

//    stream << "matrix[" << matrix.size().matrixWidth() << "x" << matrix.size().matrixHeight()
//           << "] with #entries = " << matrix.entries.size() << ", nnz = " << matrix.size().nnz() << endl;

    for (int i = 0; i < matrix.size().pRow(); ++i) {
        for (int j = 0; j < matrix.size().matrixWidth(); ++j)
            stream << "\t" << "_";
        stream << "\n";
    }

    int rowBeginIncl = matrix.rowIntervals[matrix.size().pRow()], rowEndExcl;
    for (int rowIdx = matrix.size().pRow(); rowIdx < matrix.size().kRow(); ++rowIdx) {
        rowEndExcl = matrix.rowIntervals[rowIdx+1];

        int colIdx;
        for (colIdx = 0; colIdx < matrix.size().pCol(); ++colIdx)
            stream << "\t" << "_";

        for (int itemIdx = rowBeginIncl; itemIdx < rowEndExcl; ++itemIdx) {
            while (colIdx < matrix.columns[itemIdx]) {
                stream << "\t" << 0.0;
                ++colIdx;
            }
            stream << "\t" << matrix.entries[itemIdx];
            ++colIdx;
        }
        while (colIdx < matrix.size().kCol()) {
            stream << "\t" << 0.0;
            ++colIdx;
        }
        for (; colIdx < matrix.size().matrixWidth(); ++colIdx)
            stream << "\t" << "_";

        stream << "\n";
        rowBeginIncl = rowEndExcl;
    }

    for (int i = matrix.size().kRow(); i < matrix.size().matrixHeight(); ++i) {
        for (int j = 0; j < matrix.size().matrixWidth(); ++j)
            stream << "\t" << "_";
        stream << "\n";
    }

    return stream;
}

const SparseMatrixFragment::SparseMatrixFragmentDescriptor &SparseMatrixFragment::size() const {
    return size_;
}

void SparseMatrixFragment::loadFromFile(istream &matrixSource) {
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
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::mergeRows(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat) {

    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);
    setMergeDimensions(fragmentsToConcat, res->size_);

//    cout << "((1)) preparing to make merged matrix:" << endl;
//    cout << "width: " << res->size().matrixWidth()
//            << ", height: " << res->size().matrixHeight()
//            << ", p = " << res->size().pRow() << ", " << res->size().pCol()
//            << ", k = " << res->size().kRow() << ", " << res->size().kCol() << endl;

    res->size_.nnz_ = 0;
    res->rowIntervals.push_back(0);
    for (int rowIdx = 0; rowIdx < res->size().pRow(); ++rowIdx) {
        res->rowIntervals.push_back(0);
    }

    for (size_t partIdx = 0; partIdx < fragmentsToConcat.size(); ++partIdx) {
        auto const &part = fragmentsToConcat[partIdx];
        for (int rowIdx = part->size().pRow(); rowIdx < part->size().kRow(); ++rowIdx) {

            const int rowBeginIncl = part->rowIntervals[rowIdx];
            const int rowEndExcl = part->rowIntervals[rowIdx + 1];

            for (int entryIdx = rowBeginIncl; entryIdx < rowEndExcl; ++entryIdx) {
                res->entries.push_back(part->entries[entryIdx]);
                res->columns.push_back(part->columns[entryIdx]);
            }

            res->rowIntervals.push_back((int) res->entries.size());
            ++res->size_.nnz_;
        }
    }

    for (int rowIdx = res->size().kRow(); rowIdx < res->size().matrixHeight(); ++rowIdx)
        res->rowIntervals.push_back(0);

    return res;
}

std::shared_ptr<SparseMatrixFragment> SparseMatrixFragment::mergeCols(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat) {

    std::shared_ptr<SparseMatrixFragment> res = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment);
    setMergeDimensions(fragmentsToConcat, res->size_);

//    cout << "((1)) preparing to make merged matrix:" << endl;
//    cout << "width: " << res->size().matrixWidth()
//            << ", height: " << res->size().matrixHeight()
//            << ", p = " << res->size().pRow() << ", " << res->size().pCol()
//            << ", k = " << res->size().kRow() << ", " << res->size().kCol() << endl;

    res->rowIntervals.push_back(0);
    for (int rowIdx = 0; rowIdx < res->size().matrixHeight(); ++rowIdx) {
        for (size_t partIdx = 0; partIdx < fragmentsToConcat.size(); ++partIdx) {
            auto const &part = fragmentsToConcat[partIdx];
            for (int entryIdx = part->rowIntervals[rowIdx]; entryIdx < part->rowIntervals[rowIdx+1]; ++entryIdx) {
                res->entries.push_back(part->entries[entryIdx]);
                res->columns.push_back(part->columns[entryIdx]);
            }
        }
        res->rowIntervals.push_back((int) res->entries.size());
    }

    return res;
}

void SparseMatrixFragment::setMergeDimensions(const std::vector<std::shared_ptr<SparseMatrixFragment>> &fragmentsToConcat,
                                              SparseMatrixFragmentDescriptor &descr) {

    assert(fragmentsToConcat.size() > 0);

    const auto & size = fragmentsToConcat[0]->size();
    descr.matrixWidth(size.matrixWidth());
    descr.matrixHeight(size.matrixHeight());

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

    descr.pCol(minPCol);
    descr.pRow(minPRow);
    descr.kCol(maxKCol);
    descr.kRow(maxKRow);
}

void SparseMatrixFragment::splitIntoColumnGroups(
        const std::vector<int> &rowDivision,
        std::vector<std::shared_ptr<SparseMatrixFragment>> &res) const {

    assert(rowIntervals.size() > 0);
    assert(!size().isFragment());
    assert(rowDivision[rowDivision.size() - 1] == size().matrixWidth());

    res.resize(rowDivision.size() - 1);

    int numBlocks = static_cast<int>(rowDivision.size() - 1);
    for (int i = 0; i < numBlocks; ++i) {
        res[i] = make_shared<SparseMatrixFragment>(size());
        res[i]->size_.pCol(rowDivision[i]);
        res[i]->size_.kCol(rowDivision[i+1]);
        res[i]->rowIntervals.push_back(0);
        res[i]->size_.nnz_ = 0;
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

            res[dividerIdx]->entries.push_back(entries[elEntryIdx]);
            res[dividerIdx]->columns.push_back(columns[elEntryIdx]);
            res[dividerIdx]->size_.nnz_++;
        }

        // Finalize each row entry in each submatrix
        for (auto &resItem : res) {
            resItem->rowIntervals.push_back(static_cast<int>(resItem->entries.size()));
        }
        rowBeginIncl = rowEndExcl;
    }
}

SparseMatrixFragment::SparseMatrixFragment(const SparseMatrixFragmentDescriptor &descriptor) {
    size_ = descriptor;
}

void SparseMatrixFragment::splitIntoRowGroups(const vector<int> &columnDivision, vector<shared_ptr<SparseMatrixFragment>> &res) const {
    assert((int) rowIntervals.size() - 1 == size().matrixHeight());
    assert(!size().isFragment());
    assert(columnDivision[columnDivision.size() - 1] == size().matrixHeight());

    res.resize(columnDivision.size() - 1);

    int numBlocks = static_cast<int>(columnDivision.size() - 1);
    for (int i = 0; i < numBlocks; ++i) {
        res[i] = make_shared<SparseMatrixFragment>(size());
        res[i]->size_.pRow(columnDivision[i]);
        res[i]->size_.kRow(columnDivision[i+1]);
        res[i]->size_.nnz_ = 0;
        res[i]->rowIntervals.push_back(0);
    }

    int rowBeginIncl = rowIntervals[0], rowEndExcl;
    int dividerIdx = 0;

    // Iterate by rows.
    for (int i = 0; i < size().matrixHeight(); ++i) {
        rowEndExcl = rowIntervals[i+1];

        if (i >= columnDivision[dividerIdx+1])
            ++dividerIdx;

        // Read one row and copy its elements to appropriate submatrices.
        for (int elEntryIdx = rowBeginIncl; elEntryIdx < rowEndExcl; ++elEntryIdx) {
            // Current element is in entries[elEntryIdx]; columns[elEntryIdx];
            res[dividerIdx]->entries.push_back(entries[elEntryIdx]);
            res[dividerIdx]->columns.push_back(columns[elEntryIdx]);
            res[dividerIdx]->size_.nnz_++;
        }

        // Finalize each row entry in each submatrix
        for (auto &resItem : res) {
            resItem->rowIntervals.push_back(static_cast<int>(resItem->entries.size()));
        }
        rowBeginIncl = rowEndExcl;
    }
}

void SparseMatrixFragment::splitIntoGroups(const std::vector<int> division, vector<shared_ptr<SparseMatrixFragment>> &res,
                                           bool groupRows) const {
    if (groupRows)
        splitIntoRowGroups(division, res);
    else
        splitIntoColumnGroups(division, res);
}


int SparseMatrixFragment::SparseMatrixFragmentDescriptor::nnz() const {
    return nnz_;
}

int SparseMatrixFragment::SparseMatrixFragmentDescriptor::maxItemsPerRow() const {
    return maxItemsPerRow_;
}


