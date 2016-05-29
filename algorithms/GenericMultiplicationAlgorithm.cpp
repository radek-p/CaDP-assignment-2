//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "GenericMultiplicationAlgorithm.h"


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(
        const string &matrixASourceFile, int matrixBSeed, int exponent, int c) :
        matrixASourceFile(matrixASourceFile), matrixBSeed(matrixBSeed), exponent(exponent), c(c) {

}

/* - Implementation of op level steps of algorithm ------------------------------------------------------------------ */

void GenericMultiplicationAlgorithm::step1_prepareData() {
    // scatter sparse matrix;
    scatterSparseMatrixA();
    // cache sparse matrix;
    cacheSparseMatrixA();
    // cache dense matrix
    cacheDenseMatrixA();
}

void GenericMultiplicationAlgorithm::step3_printResults(bool printMatrix, bool printGeCounter, double geTreshold) {
    if (printMatrix) {
        // FIXME: replace the following line: print the whole result matrix
        gatherResultMatrix();
        printf("1 1\n42\n");
    }

    if (printGeCounter) {
        countGreaterThan(geTreshold);
        // FIXME: replace the following line: count ge elements
        printf("54\n");
    }
}

/* ------------------------------------------------------------------------------------------------------------------ */

void GenericMultiplicationAlgorithm::scatterSparseMatrixA() {

}

void GenericMultiplicationAlgorithm::cacheSparseMatrixA() {

}

void GenericMultiplicationAlgorithm::cacheDenseMatrixA() {

}

void GenericMultiplicationAlgorithm::countGreaterThan(double treshold) {

}

void GenericMultiplicationAlgorithm::gatherResultMatrix() {

}
