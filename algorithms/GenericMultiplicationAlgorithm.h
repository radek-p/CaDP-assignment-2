//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
#define MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H


#include <string>
#include "../utility/SparseMatrix.h"

using namespace std;

class GenericMultiplicationAlgorithm {
public:
    GenericMultiplicationAlgorithm(const string &matrixASourceFile, int matrixBSeed, int exponent, int c);

    void step1_prepareData();

    virtual void step2_performMultiplication() = 0;

    void step3_printResults(bool printMatrix, bool printGeCounter, double geTreshold);

    void scatterSparseMatrixA();

    void cacheSparseMatrixA();

    void cacheDenseMatrixA();

    void countGreaterThan(double treshold);

    void gatherResultMatrix();

private:
    std::string matrixASourceFile;
    int matrixBSeed;
    int exponent;
    int c;
};


#endif //MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
