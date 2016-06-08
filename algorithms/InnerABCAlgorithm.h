//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_INNERABCALGORITHM_H
#define MATRIXMUL_INNERABCALGORITHM_H

#include "GenericMultiplicationAlgorithm.h"


class InnerABCAlgorithm : public GenericMultiplicationAlgorithm {
public:
    InnerABCAlgorithm(int c);

    virtual bool splitAInRowGroups();

    virtual void step4_redistributeMatrixA();

    virtual void step5_redistributeMatrixB();

    virtual void step6_performSingleMultiplication();

    virtual void step7_setResultAsNewBMatrix();

    virtual void step8_countAndPrintGe(double geElement);

    virtual void step9_printResultMatrix();

protected:
    boost::mpi::communicator replicationGroup;
    boost::mpi::communicator orthogonalGroup;
    std::vector<std::shared_ptr<DenseMatrixFragment>> C;

    int q() const { return p() / (replicationFactor_ * replicationFactor_); };
    int l() const { return replicationGroup.rank(); };

    void shiftMatrixA();
};


#endif //MATRIXMUL_INNERABCALGORITHM_H
