//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_INNERABCALGORITHM_H
#define MATRIXMUL_INNERABCALGORITHM_H

#include "GenericMultiplicationAlgorithm.h"


class InnerABCAlgorithm : public GenericMultiplicationAlgorithm {
public:
    InnerABCAlgorithm(int c) :
            GenericMultiplicationAlgorithm(c) { }

    virtual void step4_redistributeMatrixA();

    virtual void step5_redistributeMatrixB();

    virtual void step6_performSingleMultiplication();

    virtual void step7_setResultAsNewBMatrix();


protected:
    int q() const { return p() / (replicationFactor_ * replicationFactor_); };
    int l() const { return 42; }; // TODO implement

};


#endif //MATRIXMUL_INNERABCALGORITHM_H
