//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "SparseMatrix.h"

#include <iostream>
#include <fstream>


SparseMatrix::SparseMatrix(std::string sourceFile, bool storeInColumnMajorOrder) {
    loadFromFile(sourceFile, storeInColumnMajorOrder);
}

double SparseMatrix::at(int i, int j) const {
    if (!rowMajor)
        swap(i, j);

    map<int, double>::const_iterator iterator    = data[i].find(j);
    map<int, double>::const_iterator endIterator = data[i].end();

    return (iterator == endIterator) ? 0.0 : iterator->second;
}

std::ostream &operator<<(std::ostream &stream, const SparseMatrix &matrix) {
    for (int rowIdx = 0; rowIdx < matrix.height; ++rowIdx) {
        if (matrix.width > 0) {
            stream << matrix.at(rowIdx, 0);
        }
        for (int colIdx = 1; colIdx < matrix.width; ++colIdx) {
            stream << "\t" << matrix.at(rowIdx, colIdx);
        }
        stream << "\n";
    }
    return stream;
}

void SparseMatrix::loadFromFile(std::string sourceFile, bool storeInColumnMajorOrder) {
    ifstream matrixSource;
    matrixSource.open(sourceFile);
    matrixSource >> width >> height >> nnz >> maxItemsPerRow;

    vector<double> nnzEntries;

    double nnzEntry;
    for (int i = 0; i < nnz; ++i) {
        matrixSource >> nnzEntry;
        nnzEntries.push_back(nnzEntry);
    }
    vector<int> rowIntervals;

    int tmp2;
    for (int i = 0; i <= height; ++i) {
        matrixSource >> tmp2;
        rowIntervals.push_back(tmp2);
    }

    int tmpColIdx;
    if (storeInColumnMajorOrder) {
        rowMajor = false;
        int nnzEntryIdx = 0;
        for (int colIdx = 0; colIdx < width; ++colIdx) {
            data.push_back(map<int, double>());
        }

        for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
            const int size = rowIntervals[rowIdx + 1] - rowIntervals[rowIdx];
            for (int rowElemIdx = 0; rowElemIdx < size; ++rowElemIdx) {
                matrixSource >> tmpColIdx;
                data[tmpColIdx][rowIdx] = nnzEntries[nnzEntryIdx];
                nnzEntryIdx++;
            }
        }
    } else {
        rowMajor = true;
        int nnzEntryIdx = 0;
        for (int rowIdx = 0; rowIdx < height; ++rowIdx) {
            data.push_back(map<int, double>());
            int size = rowIntervals[rowIdx + 1] - rowIntervals[rowIdx];
            for (int rowElemIdx = 0; rowElemIdx < size; ++rowElemIdx) {
                matrixSource >> tmpColIdx;
                data[rowIdx][tmpColIdx] = nnzEntries[nnzEntryIdx];
                nnzEntryIdx++;
            }
        }
    }

    matrixSource.close();
}





