//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "InnerABCAlgorithm.h"


void InnerABCAlgorithm::step4_redistributeMatrixA() {
    for (int i = 0; i < q() * l(); ++i) {
        shiftMatrixA();
    }
}

void InnerABCAlgorithm::step6_performSingleMultiplication() {
    for (int i = 0; i < q(); ++i) {
        *C = A * B;
        shiftMatrixA(); // TODO Maybe not perform last shift
    }
}
