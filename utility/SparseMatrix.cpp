//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "SparseMatrix.h"

#include <iostream>
#include <fstream>


SparseMatrix::SparseMatrix(std::string sourceFile, bool storeInColumnMajorOrder) {
    ifstream matrixSource;
    matrixSource.open(sourceFile);
    matrixSource >> width >> height >> nnz >> maxItemsPerRow;

    vector<double> nnzEntries;

    double tmp;
    for (u_int32_t i = 0; i < nnz; ++i) {
        matrixSource >> tmp;
        nnzEntries.push_back(tmp);
    }

    vector<u_int32_t> rowIntervals;

    u_int32_t tmp2;
    for (u_int32_t i = 0; i <= height; ++i) {
        matrixSource >> tmp2;
        rowIntervals.push_back(tmp2);
    }

    if (storeInColumnMajorOrder) {
        rowMajor = false;
        cerr << "TODO implement" << endl;
    } else {
        rowMajor = true;

        u_int32_t nnzEntryIdx = 0;
        for (u_int32_t rowIdx = 0; rowIdx < height; ++rowIdx) {
            const u_int32_t size = rowIntervals[rowIdx+1] - rowIntervals[rowIdx];
            data.push_back(vector<pair<u_int32_t, double>>(size));
            for (u_int32_t rowElemIdx = 0; rowElemIdx < size; ++rowElemIdx) {
                cin >> tmp2;
                data[rowIdx][rowElemIdx] = make_pair(tmp2, nnzEntries[nnzEntryIdx]);
                nnzEntryIdx++;
            }
        }
    }

    matrixSource.close();
}

void SparseMatrix::print() {
    if (rowMajor) {
        for (u_int32_t rowIdx = 0; rowIdx < height; ++rowIdx) {
            u_int32_t nextCol = 0;
            for (u_int32_t rowEltIdx = 0; rowEltIdx < data[rowIdx].size(); ++rowEltIdx) {
                for (; nextCol < data[rowIdx][rowEltIdx].first; ++nextCol) {
                    cout << "\t0";
                }
                cout << "\t" << data[rowIdx][rowEltIdx].second;
                nextCol++;
            }
            for (; nextCol < width; ++nextCol) {
                cout << "\t0";
            }
            cout << "\n";
        }
    } else {
        cerr << "TODO implement" << endl;
    }
}



