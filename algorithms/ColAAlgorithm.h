//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_COLAALGORITHM_H
#define MATRIXMUL_COLAALGORITHM_H

#include "GenericMultiplicationAlgorithm.h"


class ColAAlgorithm : public GenericMultiplicationAlgorithm {
public:
    ColAAlgorithm(int c) :
            GenericMultiplicationAlgorithm(c) { }

    virtual void step4_redistributeMatrixA();

    virtual void step6_performSingleMultiplication();

    virtual void step7_setResultAsNewBMatrix();

    virtual void step8_countAndPrintGe(double geElement);

    virtual void step9_printResultMatrix();

    virtual bool splitAInRowGroups();

    void shiftMatrixA();

protected:
    std::shared_ptr<DenseMatrixFragment> C;
};

#endif //MATRIXMUL_COLAALGORITHM_H
