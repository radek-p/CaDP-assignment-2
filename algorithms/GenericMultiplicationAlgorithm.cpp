//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include <mpi.h>
#include <iostream>

#include "GenericMultiplicationAlgorithm.h"


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(
        const string &matrixASourceFile, int matrixBSeed, int exponent, int c
) :
        matrixASourceFile(matrixASourceFile), matrixBSeed(matrixBSeed), exponent(exponent), c(c)
{

    // Get information about size and position in MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &numProcGlobal);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankGlobal);

    cout << "my rank: " << rankGlobal << endl;
}

/* - Implementation of op level steps of algorithm ------------------------------------------------------------------ */

void GenericMultiplicationAlgorithm::step1_prepareData() {
    prepareInitialDistributionOfMatrices();
}

void GenericMultiplicationAlgorithm::step3_printResults(bool printMatrix, bool printGeCounter, double geThreshold) {
    if (printMatrix) {
//        // replace the following line: print the whole result matrix
//        gatherResultMatrix();
//        printf("1 1\n42\n");
    }

    if (printGeCounter) {
//        countGreaterThan(geThreshold);
//        // replace the following line: count ge elements
//        printf("54\n");
    }
}

void GenericMultiplicationAlgorithm::countGreaterThan(double treshold) {

}

void GenericMultiplicationAlgorithm::gatherResultMatrix() {

}

bool GenericMultiplicationAlgorithm::isCoordinator() {
    return rankGlobal == 0;
}

int GenericMultiplicationAlgorithm::getFirstIdx(
        PARTITION_TYPE partitionType, int j, int matrixSize, int p, int c
) {
    if (partitionType == PARTITION_TYPE::P_DIV_C_BLOCKS) {
        int p_div_c = p / c;
        return j * (matrixSize / p_div_c) + min(j, matrixSize % p_div_c);
    } else { // PARTITION_TYPE::P_BLOCKS
        int superBlockFirstIncl = getFirstIdx(PARTITION_TYPE::P_DIV_C_BLOCKS, j % c,     matrixSize, p, c);
        int superBlockLastExcl  = getFirstIdx(PARTITION_TYPE::P_DIV_C_BLOCKS, j % c + 1, matrixSize, p, c);
        int superBlockSize = superBlockLastExcl - superBlockFirstIncl;
        return superBlockFirstIncl + (j % c) * (superBlockSize / c) + min(j % c, superBlockSize % c);
    }
}








