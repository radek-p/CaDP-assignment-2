//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include <mpi.h>
#include <iostream>

#include "GenericMultiplicationAlgorithm.h"
#include "densematgen.h"


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(int c ) : replicationFactor_(c) {
    // Get information about size and position in MPI_COMM_WORLD
    MPI_Comm_size(MPI_COMM_WORLD, &numProcGlobal_);
    MPI_Comm_rank(MPI_COMM_WORLD, &rankGlobal_);

    cout << "my rank: " << rankGlobal_ << endl;
}

/* - Implementation of op level steps of algorithm ------------------------------------------------------------------ */

bool GenericMultiplicationAlgorithm::isCoordinator() {
    return isCoordinator(rankGlobal_);
}

bool GenericMultiplicationAlgorithm::isCoordinator(int rankGlobal) {
    return rankGlobal == 0;
}

int GenericMultiplicationAlgorithm::getFirstIdx(
        int j, int matrixSize, int p
) {
    // Example: p = 6, matrixSize = 39 = 6 * p + 3
    // |xxxxxx|xxxxxx|xxxxxx|xxxxxx|xxxxxx|xxxxxx|xxx|
    //                                            ^^^
    // remaining 3 elements should be distributed
    // uniformly into p blocks:
    // |xxxxxx|xxxxxxx|xxxxxx|xxxxxxx|xxxxxx|xxxxxxx||
    //         ^              ^              ^
    //         ----- blocks where extra  -----
    //               elements were added
    // j:
    // |   0   |  1   |   2   |  3   |   4   |  5   ||  6
    // j * 3 / p:
    // |   0   |  0   |   1   |  1   |   2   |  2   ||  3

    int baseBlockSize  = matrixSize / p;
    int remainingCount = matrixSize % p; // number in {0, 1, 2, ..., p-1}
    int jthBlockFirstIncl = (baseBlockSize * j) + ((remainingCount * j) / p);

    // Note that
    // for j = 0: remainingCount * j / p = 0
    // for j = p: remainingCount * p / p = remainingCount
    // so all elements are uniformly distributed.

    return jthBlockFirstIncl;
}

void GenericMultiplicationAlgorithm::step1_loadMatrixA(const string &fileName) {

}

void GenericMultiplicationAlgorithm::step2_distributeMatrixA() {
    // TODO receive A size
}

void GenericMultiplicationAlgorithm::step3_generateMatrixB(int seed) {
    MatrixFragment::MatrixFragmentDescriptor dscr = size_;

    dscr.fragmentHeight(dscr.matrixHeight());
    dscr.fragmentWidth(dscr.matrixWidth());

    int firstColIncl = getFirstIdx(j(), dscr.matrixWidth(), p());
    int lastColExcl  = getFirstIdx(j()+1, dscr.matrixWidth(), p());

    dscr.pCol(firstColIncl);
    dscr.kCol(lastColExcl);

    B = make_shared<DenseMatrix>(dscr);

    for (int i = B->size().pRow(); i < B->size().kRow(); ++i)
        for (int j = B->size().pCol(); j < B->size().kCol(); ++j)
            B->at(i, j) = generate_double(seed, i, j);
}

void GenericMultiplicationAlgorithm::step7_setResultAsNewBMatrix() {

}

void GenericMultiplicationAlgorithm::step8_countAndPrintGe(double geElement) {
    int counter = 0;

    for (int i = C->size().pRow(); i < C->size().kRow(); ++i)
        for (int j = C->size().pCol(); j < C->size().kCol(); ++j)
            if (C->at(i, j) >= geElement)
                ++counter;

    // TODO Reduce counter value and print it on master
}

void GenericMultiplicationAlgorithm::step9_printResultMatrix() {

}

void GenericMultiplicationAlgorithm::shiftMatrixA() {

}










