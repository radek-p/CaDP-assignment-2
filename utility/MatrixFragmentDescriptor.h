//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#ifndef MATRIXMUL_MATRIXFRAGMENTDESCRIPTOR_H
#define MATRIXMUL_MATRIXFRAGMENTDESCRIPTOR_H


#include <cstddef>

class MatrixFragmentDescriptor {
public:
    // Size of the whole matrix
    const size_t sizeX, sizeY;

    // FIXME Decide if this is needed
    const size_t rowOffset;

    // Position of the top left corner of the fragment
    const size_t pX, pY;

    // Width and height of the matrix fragment
    const size_t wX, wY;

    // Position of the bottom right corner (exclusive)
    const size_t kX, kY;

    MatrixFragmentDescriptor(size_t sizeX, size_t sizeY, size_t pX, size_t pY, size_t wX, size_t wY) :
            sizeX(sizeX), sizeY(sizeY), rowOffset(0), pX(pX), pY(pY), wX(wX), wY(wY), kX(pX+wX), kY(pY+wY) { };
};


#endif //MATRIXMUL_MATRIXFRAGMENTDESCRIPTOR_H
