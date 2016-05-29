//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_COLAALGORITHM_H
#define MATRIXMUL_COLAALGORITHM_H

#include "GenericMultiplicationAlgorithm.h"


class ColAAlgorithm : public GenericMultiplicationAlgorithm {
public:
    ColAAlgorithm(const string &matrixASourceFile, int matrixBSeed, int exponent, int c) :
            GenericMultiplicationAlgorithm(matrixASourceFile, matrixBSeed, exponent, c) { }

    virtual void step2_performMultiplication() override;

protected:
    virtual void computeInitialDataDistribution() override;
};


#endif //MATRIXMUL_COLAALGORITHM_H
