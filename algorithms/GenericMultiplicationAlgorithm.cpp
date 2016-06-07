//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include <iostream>
#include <boost/mpi/collectives.hpp>

#include "GenericMultiplicationAlgorithm.h"
#include "densematgen.h"


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(int c ) : replicationFactor_(c) {
    // Get information about size and position in MPI_COMM_WORLD
//    MPI_Comm_size(MPI_COMM_WORLD, &numProcGlobal_);
//    MPI_Comm_rank(MPI_COMM_WORLD, &rankGlobal_);
    cout.precision(5);
//    cout << "my rank: " << rankGlobal_ << endl;
}

/* - Implementation of op level steps of algorithm ------------------------------------------------------------------ */

bool GenericMultiplicationAlgorithm::isCoordinator() {
    return isCoordinator(world.rank());
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
    if (isCoordinator()) {
        wholeA = SparseMatrixFragment::loadFromFile(fileName);
    }
}

void GenericMultiplicationAlgorithm::step3_generateMatrixB(int seed) {
    MatrixFragment::MatrixFragmentDescriptor dscr(A->size().matrixHeight(), A->size().matrixWidth());

    int firstColIncl = getFirstIdx(j(), dscr.matrixWidth(), p());
    int lastColExcl  = getFirstIdx(j()+1, dscr.matrixWidth(), p());

    dscr.pCol(firstColIncl);
    dscr.kCol(lastColExcl);

    B = make_shared<DenseMatrix>(dscr);

    cout << "B width: " << B->size().matrixWidth() << endl;

    for (int i = B->size().pRow(); i < B->size().kRow(); ++i)
        for (int j = B->size().pCol(); j < B->size().kCol(); ++j)
            B->at(i, j) = generate_double(seed, i, j);

    for (int i = 0; i < world.size(); ++i) {
        if (world.rank() == i) {
            cout << "generated[" << world.rank() << "]" << endl;
            cout << *B << endl;
        }
        world.barrier();
    }
}

void GenericMultiplicationAlgorithm::step7_setResultAsNewBMatrix() {

}

void GenericMultiplicationAlgorithm::step8_countAndPrintGe(double geElement) {

    int localCount = B->countGreaterOrEqual(geElement); // TODO swith to C
    int globalCount = 0;

    boost::mpi::reduce(world, localCount, globalCount, std::plus<int>(), 0);

    if (isCoordinator()) {
        cout << "GE count: " << globalCount;
    }
}

void GenericMultiplicationAlgorithm::step9_printResultMatrix() {

}

void GenericMultiplicationAlgorithm::shiftMatrixA() {

}

std::vector<int> GenericMultiplicationAlgorithm::prepareDivision(int matrixSize, int p) {
    vector<int> res((size_t) (p + 1));

    for (int i = 0; i <= p; ++i)
        res[i] = getFirstIdx(i, matrixSize, p);

    return res;
}











