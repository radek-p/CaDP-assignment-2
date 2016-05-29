#
#         autor: Radosław Piórkowski
# numer indeksu: 335451
#

cmake_minimum_required(VERSION 3.4)
project(matrixmul)

# Find MPI package
set(MPI_CXX_COMPILER /opt/local/bin/mpicxx-mpich-mp)
set(MPI_C_COMPILER   /opt/local/bin/mpicc-mpich-mp)
find_package(MPI REQUIRED)

# TODO remove -Wno-??? flags later, when the project is more mature
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field")

include_directories(SYSTEM ${MPI_INCLUDE_PATH})

set(
        SOURCE_FILES
        matrixmul.cpp
        algorithms/GenericMultiplicationAlgorithm.cpp
        algorithms/GenericMultiplicationAlgorithm.h
        algorithms/ColAAlgorithm.cpp
        algorithms/ColAAlgorithm.h algorithms/InnerABCAlgorithm.cpp algorithms/InnerABCAlgorithm.h utility/SparseMatrix.cpp utility/SparseMatrix.h utility/logging.h utility/MatrixFragmentDescriptor.cpp utility/MatrixFragmentDescriptor.h)
add_executable(matrixmul ${SOURCE_FILES})
target_link_libraries(matrixmul ${MPI_C_LIBRARIES})