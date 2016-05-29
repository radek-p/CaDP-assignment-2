//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_INNERABCALGORITHM_H
#define MATRIXMUL_INNERABCALGORITHM_H

#include "GenericMultiplicationAlgorithm.h"


class InnerABCAlgorithm : public GenericMultiplicationAlgorithm {
public:
    InnerABCAlgorithm(const string &matrixASourceFile, int matrixBSeed, int exponent, int c) :
            GenericMultiplicationAlgorithm(matrixASourceFile, matrixBSeed, exponent, c) { }

    virtual void step2_performMultiplication() override {

    }

protected:
    virtual void computeInitialDataDistribution() override;

};


#endif //MATRIXMUL_INNERABCALGORITHM_H
