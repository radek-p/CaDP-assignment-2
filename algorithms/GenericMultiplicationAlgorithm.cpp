//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include <mpi.h>

#include "GenericMultiplicationAlgorithm.h"


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(
        const string &matrixASourceFile, int matrixBSeed, int exponent, int c) :
        matrixASourceFile(matrixASourceFile), matrixBSeed(matrixBSeed), exponent(exponent), c(c) {

    // Get information about size and position in MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &numProcGlobal);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankGlobal);
}

/* - Implementation of op level steps of algorithm ------------------------------------------------------------------ */

void GenericMultiplicationAlgorithm::step1_prepareData() {
    // compute initial fragments of matrices after initial data distribution phase
    computeInitialDataDistribution();
    // scatter sparse matrix
    scatterSparseMatrixA();
    // cache sparse matrix
    cacheSparseMatrixA();
    // cache dense matrix
    cacheDenseMatrixA();
}

void GenericMultiplicationAlgorithm::step3_printResults(bool printMatrix, bool printGeCounter, double geThreshold) {
    if (printMatrix) {
        // FIXME: replace the following line: print the whole result matrix
        gatherResultMatrix();
        printf("1 1\n42\n");
    }

    if (printGeCounter) {
        countGreaterThan(geThreshold);
        // FIXME: replace the following line: count ge elements
        printf("54\n");
    }
}

/* ------------------------------------------------------------------------------------------------------------------ */

void GenericMultiplicationAlgorithm::scatterSparseMatrixA() {
    if (isCoordinator()) {
        SparseMatrix wholeA(matrixASourceFile);
        wholeA.print();
    }

    // TODO load matrices
    // TODO set matrixSize to proper value
    matrixSize = 142;
}

void GenericMultiplicationAlgorithm::cacheSparseMatrixA() {

}

void GenericMultiplicationAlgorithm::cacheDenseMatrixA() {

}

void GenericMultiplicationAlgorithm::countGreaterThan(double treshold) {

}

void GenericMultiplicationAlgorithm::gatherResultMatrix() {

}

bool GenericMultiplicationAlgorithm::isCoordinator() {
    return rankGlobal == 0;
}




