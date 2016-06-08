#
#         autor: Radosław Piórkowski
# numer indeksu: 335451
#
cmake_minimum_required(VERSION 2.8)
project(matrixmul)

# CMake commands that are commented out were used
# to compile the project locally. They were left
# here, since they may be useful for compiling
# the project not on 'students' server.

# Find MPI library
set(MPI_CXX_COMPILER /opt/local/bin/mpicxx-mpich-mp) # local
set(MPI_C_COMPILER   /opt/local/bin/mpicc-mpich-mp)  # local
find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

# Find Boost library
# There is Boost 1.6 installed on students in /tmp/boost16,
# we have to tell CMake about its location
#set(Boost_LIBRARY_DIR /tmp/boost16/lib)     # students
#set(Boost_INCLUDE_DIR /tmp/boost16/include) # students
set(Boost_LIBRARY_DIR /opt/local/lib/)      # local
find_package(Boost COMPONENTS mpi serialization REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

# Set apropriate compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

include_directories(. utility algorithms)
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
        utility/MatrixFragment.cpp
        utility/MatrixFragment.h
        utility/MatrixOperations.h
        utility/MatrixOperations.cpp
)
add_executable(matrixmul matrixmul.cpp ${SOURCE_FILES} ${FOREIGN_FILES})
target_link_libraries(matrixmul ${MPI_C_LIBRARIES} ${Boost_LIBRARIES})


