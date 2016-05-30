//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "ColAAlgorithm.h"

void ColAAlgorithm::step2_performMultiplication() {
    if (isCoordinator()) {
        SparseMatrix wholeA(matrixASourceFile, true);

        // Store my part of matrix

        for (int procIdxGlobal = 1; procIdxGlobal < numProcGlobal; ++procIdxGlobal) {
            int beginIdxIncl = getFirstIdx(PARTITION_TYPE::P_BLOCKS, procIdxGlobal    , matrixSize, numProcGlobal, c);
            int endIdxExcl   = getFirstIdx(PARTITION_TYPE::P_BLOCKS, procIdxGlobal + 1, matrixSize, numProcGlobal, c);

            // SendMatrix
        }
    } else {
        // Receive A matrix
    }
}

void ColAAlgorithm::prepareInitialDistributionOfMatrices() {
}