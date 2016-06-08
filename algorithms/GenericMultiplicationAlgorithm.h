//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
#define MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H


#include <string>
#include <memory>
#include <boost/mpi/communicator.hpp>

#include "../utility/SparseMatrixFragment.h"
#include "../utility/MatrixFragment.h"
#include "../utility/DenseMatrixFragment.h"


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
//#ifdef DEBUG
//    static const bool DEBUG = true;
//#else
//    static const bool DEBUG = false;
//#endif
public:
    GenericMultiplicationAlgorithm(int c);

    void step1_loadMatrixA(const std::string &fileName);
    void step2_distributeMatrixA();
    void step3_generateMatrixB(int seed);
    virtual void step4_redistributeMatrixA() = 0;
    virtual void step5_redistributeMatrixB() { };
    virtual void step6_performSingleMultiplication() = 0;
    virtual void step7_setResultAsNewBMatrix() = 0;
    virtual void step8_countAndPrintGe(double geElement) = 0;
    virtual void step9_printResultMatrix() = 0;

    bool isCoordinator();
    static bool isCoordinator(int rankGlobal);

    // Partitions interval matrixSize into p blocks of similar size.
    static int getFirstIdx(int blockIdx, int matrixSize, int p);

    static std::vector<int> prepareDivision(int matrixSize, int p);

    // Procedure decides whether algorithm should split matrix A into column or row groups.
    virtual bool splitAInRowGroups() = 0;

    enum tags {
        REDISTRIBUTE_A = 23,
        SHIFT_A        = 42,
    };

protected:
    // Replication factor c
    int replicationFactor_;

    boost::mpi::communicator world;

    int c()     const { return replicationFactor_; };
    int p()     const { return world.size(); };
    int pDivC() const { return world.size() / replicationFactor_; };
    int j()     const { return world.rank(); };

    // Pointer to whole matrix A, is is used only by
    // the coordinator and is destroyed in step2
    std::shared_ptr<SparseMatrix> wholeA;

    // Initial part of A matrix, its size is
    std::shared_ptr<SparseMatrixFragment> initialAPart;

    // Pointers to parts of respective matrices.
    // In InnerABC size of each part is 1/(p/c) of size of the whole matrix,
    // but in ColA size of B is 1/p of size of whole B.
    std::shared_ptr<SparseMatrixFragment> A;
    std::shared_ptr<DenseMatrixFragment>  B;
};

#endif //MATRIXMUL_GENERICMULTIPLICATIONALGORITHM_H
