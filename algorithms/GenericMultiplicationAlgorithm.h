//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
#define MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H


#include <string>
#include <memory>

#include "../utility/SparseMatrixFragment.h"
#include "../utility/MatrixFragment.h"
#include "../utility/DenseMatrixFragment.h"

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
    GenericMultiplicationAlgorithm(int c);

    void step1_loadMatrixA(const string &fileName);
    void step2_distributeMatrixA();
    void step3_generateMatrixB(int seed);
    virtual void step4_redistributeMatrixA() = 0;
    virtual void step5_redistributeMatrixB() { };
    virtual void step6_performSingleMultiplication() = 0;
    void step7_setResultAsNewBMatrix();
    void step8_countAndPrintGe(double geElement);
    void step9_printResultMatrix();

    bool isCoordinator();
    static bool isCoordinator(int rankGlobal);

    // Partitions interval matrixSize into p blocks of similar size.
    static int getFirstIdx(int blockIdx, int matrixSize, int p);

protected:
    MatrixFragment::MatrixFragmentDescriptor size_;
    // Replication factor c
    int replicationFactor_;
    // int matrixSize;
    int numReplicationGroups;

    int rankGlobal_    = INVALID_PARAMETER_VALUE;
    int numProcGlobal_ = INVALID_PARAMETER_VALUE;

    int c()     const { return replicationFactor_; };
    int p()     const { return numProcGlobal_;     };
    int pDivC() const { return numProcGlobal_ / replicationFactor_; };
    int j()     const { return rankGlobal_; };

    //    int rankReplicationGroup    = INVALID_PARAMETER_VALUE;
    //    int numProcReplicationGroup = INVALID_PARAMETER_VALUE;
    //
    //    int rankRowGroup    = INVALID_PARAMETER_VALUE;
    //    int numProcRowGroup = INVALID_PARAMETER_VALUE;

    // Pointer to whole matrix A, is is used only by
    // the coordinator and is destroyed in step2
    shared_ptr<SparseMatrix> wholeA;

    // Initial part of A matrix, its size is
    shared_ptr<SparseMatrixFragment> initialAPart;

    // Pointers to parts of respective matrices.
    // In InnerABC size of each part is 1/(p/c) of size of the whole matrix,
    // but in ColA size of B is 1/p of size of whole B.
    shared_ptr<SparseMatrixFragment> A;
    shared_ptr<DenseMatrixFragment>  B, C;

    void shiftMatrixA();
};

#endif //MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
