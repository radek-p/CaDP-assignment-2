//
//         autor: Radosław Piórkowski
// numer indeksu: 335451
//

#include "MatrixFragment.h"

#include <cstdlib>

//MatrixFragment::MatrixFragment(size_t bufferSize) {
////    __data = std::make_sharedmalloc(bufferSize);
//}

long send() {
    return 0;
}

int MatrixFragment::MatrixFragmentDescriptor::matrixHeight() const { return matrixHeight_;   }

int MatrixFragment::MatrixFragmentDescriptor::matrixWidth() const { return matrixWidth_;    }

int MatrixFragment::MatrixFragmentDescriptor::pRow() const { return pRow_;           }

int MatrixFragment::MatrixFragmentDescriptor::pCol() const { return pCol_;           }

int MatrixFragment::MatrixFragmentDescriptor::fragmentHeight() const { return fragmentHeight_; }

int MatrixFragment::MatrixFragmentDescriptor::fragmentWidth() const { return fragmentWidth_;  }