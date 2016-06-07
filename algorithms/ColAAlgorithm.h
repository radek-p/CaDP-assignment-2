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

    virtual void step2_distributeMatrixA();

    virtual void step4_redistributeMatrixA();

    virtual void step6_performSingleMultiplication();

    virtual void step7_setResultAsNewBMatrix();


};

#endif //MATRIXMUL_COLAALGORITHM_H
