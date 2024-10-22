//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "GenericMultiplicationAlgorithm.h"

#include <iostream>
#include <sstream>
#include <boost/mpi/collectives.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>

#include "densematgen.h"

using namespace std;


GenericMultiplicationAlgorithm::GenericMultiplicationAlgorithm(int c) : replicationFactor_(c) {
    // Get information about size and position in MPI_COMM_WORLD
    cout.precision(5);
    ios_base::sync_with_stdio(false);
}

/* - Implementation of top level steps of algorithm ----------------------------------------------------------------- */

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

bool GenericMultiplicationAlgorithm::step1_loadMatrixA(const string &fileName) {
    bool matrixSuccesfullyOpened;
    if (isCoordinator()) {
        ifstream matrixSource;
        matrixSource.open(fileName);
        matrixSuccesfullyOpened = matrixSource.is_open();
        boost::mpi::broadcast(world, matrixSuccesfullyOpened, 0);
        if (!matrixSuccesfullyOpened)
            return false;
        else {
            wholeA = make_shared<SparseMatrix>();
            wholeA->loadFromFile(matrixSource);
        }
    } else {
        boost::mpi::broadcast(world, matrixSuccesfullyOpened, 0);
        if (!matrixSuccesfullyOpened)
            return false;
    }
    return true;
}

void GenericMultiplicationAlgorithm::step2_distributeMatrixA() {

    A = shared_ptr<SparseMatrixFragment>(new SparseMatrixFragment());
    int maxPartSize = -1;
    vector<char> allMatricesTogether(0);

    if (isCoordinator()) {
        vector<shared_ptr<SparseMatrixFragment>> parts(0);
        // We assume that both matrices are square, so we always prepare the division using matrix width.
        // This should be changed, when adapting the program to more general case.
        auto division = prepareDivision(wholeA->size().matrixWidth(), p());
        wholeA->splitIntoGroups(division, parts, splitAInRowGroups());

        vector<string> serializedMatrices(0);
        for (auto const &part : parts) {
            stringstream ss;
            boost::archive::binary_oarchive ar(ss, boost::archive::no_header);
            ar << *part;
            serializedMatrices.push_back(string(ss.str()));
            maxPartSize = max(maxPartSize, (int) serializedMatrices[serializedMatrices.size() - 1].length());
        }

        boost::mpi::broadcast(world, maxPartSize, 0);

        allMatricesTogether.resize(parts.size() * maxPartSize);
        for (size_t i = 0; i < parts.size(); ++i) {
            std::copy(
                    &serializedMatrices[i][0],
                    (&serializedMatrices[i][0]) + serializedMatrices[i].length(),
                    &allMatricesTogether[i * maxPartSize]
            );
        }
//        cout << "max part size: " << maxPartSize << endl;
//        cout << *wholeA << endl;
    } else {
        boost::mpi::broadcast(world, maxPartSize, 0);
    }

    string mySingleReceivedMatrix((size_t) maxPartSize, '\0');
    boost::mpi::scatter(world, &allMatricesTogether[0], &mySingleReceivedMatrix[0], maxPartSize, 0);

    stringstream ss(mySingleReceivedMatrix);
    boost::archive::binary_iarchive ar(ss, boost::archive::no_header);
    ar >> *A;

//    world.barrier();
//    for (int i = 0; i < world.size(); ++i) {
//        if (world.rank() == i) {
//            cout << "scattered[" << world.rank() << "]" << endl;
//            cout << *A << endl;
//        }
//        world.barrier();
//    }

    // Delete the wholeA matrix, it is not used anymore.
    if (isCoordinator())
        wholeA.reset();
}

void GenericMultiplicationAlgorithm::step3_generateMatrixB(int seed) {
    MatrixFragment::MatrixFragmentDescriptor dscr(A->size().matrixHeight(), A->size().matrixWidth());

    int firstColIncl = getFirstIdx(j(), dscr.matrixWidth(), p());
    int lastColExcl  = getFirstIdx(j()+1, dscr.matrixWidth(), p());

    dscr.pCol(firstColIncl);
    dscr.kCol(lastColExcl);

    B = make_shared<DenseMatrix>(dscr);

//    cout << "B width: " << B->size().matrixWidth() << endl;

    for (int i = B->size().pRow(); i < B->size().kRow(); ++i)
        for (int j = B->size().pCol(); j < B->size().kCol(); ++j)
            B->at(i, j) = generate_double(seed, i, j);

//    for (int i = 0; i < world.size(); ++i) {
//        if (world.rank() == i) {
//            cout << "generated[" << world.rank() << "]" << endl;
//            cout << *B << endl;
//        }
//        world.barrier();
//    }
}

void GenericMultiplicationAlgorithm::step7_setResultAsNewBMatrix() {

}

std::vector<int> GenericMultiplicationAlgorithm::prepareDivision(int matrixSize, int p) {
    vector<int> res((size_t) (p + 1));

    for (int i = 0; i <= p; ++i)
        res[i] = getFirstIdx(i, matrixSize, p);

    return res;
}














