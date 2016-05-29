//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_SPARSEMATRIX_H
#define MATRIXMUL_SPARSEMATRIX_H

#include <vector>
#include <string>
#include <i386/types.h>

using namespace std;

class SparseMatrix {
public:
    bool      rowMajor;
    u_int32_t maxItemsPerRow;
    u_int32_t width, height, nnz;

    vector<vector<pair<u_int32_t, double>>> data;

    SparseMatrix(std::string sourceFile, bool storeInColumnMajorOrder = false);

    void print();
};


#endif //MATRIXMUL_SPARSEMATRIX_H
