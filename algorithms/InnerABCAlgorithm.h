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

    virtual void step6_performSingleMultiplication();

protected:
    int q() const { return numProcGlobal_ / (replicationFactor_ * replicationFactor_); };
    int l() const { return 42; }; // TODO implement

};


#endif //MATRIXMUL_INNERABCALGORITHM_H
