#
#         autor: Radosław Piórkowski
# numer indeksu: 335451
#
cmake_minimum_required(VERSION 2.8)
project(matrixmul)

# Find MPI library
set(MPI_CXX_COMPILER /opt/local/bin/mpicxx-mpich-mp)
set(MPI_C_COMPILER   /opt/local/bin/mpicc-mpich-mp)
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Find Boost library
#set(CMAKE_LIBRARY_PATH /opt/local/lib {CMAKE_LIBRARY_PATH})
find_package(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# TODO remove -Wno-??? flags later, when the project is more mature
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wno-unused-parameter -Wno-unused-private-field")

INCLUDE_DIRECTORIES(. utility algorithms)
set(
        FOREIGN_FILES
        densematgen.h
        densematgen.c
)
set(
        SOURCE_FILES
        algorithms/GenericMultiplicationAlgorithm.cpp
        algorithms/GenericMultiplicationAlgorithm.h
        algorithms/ColAAlgorithm.cpp
        algorithms/ColAAlgorithm.h
        algorithms/InnerABCAlgorithm.cpp
        algorithms/InnerABCAlgorithm.h
        utility/SparseMatrixFragment.cpp
        utility/SparseMatrixFragment.h
        utility/DenseMatrixFragment.cpp
        utility/DenseMatrixFragment.h
        utility/logging.h
        utility/MatrixFragment.cpp
        utility/MatrixFragment.h
        utility/MatrixOperations.h
        utility/MatrixOperations.cpp
)
add_executable(matrixmul matrixmul.cpp ${SOURCE_FILES} ${FOREIGN_FILES})
target_link_libraries(matrixmul ${MPI_C_LIBRARIES})


