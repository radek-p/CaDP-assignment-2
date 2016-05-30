//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_SPARSEMATRIX_H
#define MATRIXMUL_SPARSEMATRIX_H

#include <vector>
#include <map>
#include <string>

using namespace std;

class SparseMatrix {
public:
    bool rowMajor;
    int  maxItemsPerRow;
    int  width, height, nnz;

    vector<map<int, double>> data;

    SparseMatrix(std::string sourceFile, bool storeInColumnMajorOrder = false);

    friend std::ostream& operator<< (std::ostream& stream, const SparseMatrix& matrix);

    double at(int i, int j) const;

private:
    void loadFromFile(std::string sourceFile, bool storeInColumnMajorOrder = false);
};


#endif //MATRIXMUL_SPARSEMATRIX_H
