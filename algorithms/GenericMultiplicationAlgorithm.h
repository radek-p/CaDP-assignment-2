//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
#define MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H


#include <string>
#include <memory>

#include "../utility/SparseMatrix.h"
#include "../utility/MatrixFragmentDescriptor.h"

using namespace std;

/**
 * Class that implements common parts of
 * both required algorithms.
 *
 * It loads matrices, partitions them and then performs
 * matrix multiplication. It holds three matrix fragments:
 *
 * A - fragment of sparse matrix
 * B - fragment of dense matrix
 * C - fragment of A*B that is being computed
 *
 * one letter variable names have the same meaning as in provided paper.
 */
class GenericMultiplicationAlgorithm {
private:
    static const int INVALID_PARAMETER_VALUE = -1;
public:
    GenericMultiplicationAlgorithm(const string &matrixASourceFile, int matrixBSeed, int exponent, int c);

    void step1_prepareData();

    virtual void step2_performMultiplication() = 0;

    void step3_printResults(bool printMatrix, bool printGeCounter, double geThreshold);

    void countGreaterThan(double treshold);

    void gatherResultMatrix();

    bool isCoordinator();

    enum class PARTITION_TYPE { P_DIV_C_BLOCKS, P_BLOCKS };

    static int getFirstIdx(PARTITION_TYPE partitionType, int blockIdx, int matrixSize, int p, int c);

private:
    int matrixBSeed;

protected:
    int exponent;
    // Replication factor c
    int c;
    int matrixSize;
    int numReplicationGroups;

    int rankGlobal    = INVALID_PARAMETER_VALUE;
    int numProcGlobal = INVALID_PARAMETER_VALUE;

    int rankReplicationGroup    = INVALID_PARAMETER_VALUE;
    int numProcReplicationGroup = INVALID_PARAMETER_VALUE;

    int rankRowGroup    = INVALID_PARAMETER_VALUE;
    int numProcRowGroup = INVALID_PARAMETER_VALUE;

    // Those descriptors describe parts of respective matrices
    shared_ptr<MatrixFragmentDescriptor> A, B, C;

    virtual void prepareInitialDistributionOfMatrices() = 0;

    std::string matrixASourceFile;
};


#endif //MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
