#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <getopt.h>
#include <string>
#include <iostream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <boost/mpi.hpp>
//#include <boost/serialization/serialization.hpp>
#pragma clang diagnostic pop

#include "algorithms/GenericMultiplicationAlgorithm.h"
#include "algorithms/ColAAlgorithm.h"
#include "algorithms/InnerABCAlgorithm.h"

using namespace std;

int main(int argc, char *argv[]) {
    /* --------------------------------------- *
     *   Adapted code from provided template   *
     * --------------------------------------- */
    bool show_results = false;
    bool use_inner = false;
    int gen_seed = -1;
    int repl_fact = 1;

    int option = -1;

    double comm_start = 0, comm_end = 0, comp_start = 0, comp_end = 0;
    int exponent = 1;
    double ge_element = 0;
    bool count_ge = false;

    boost::mpi::environment env{argc, argv};
    boost::mpi::communicator world;

    std::string matrixASourceFile = "";

    while ((option = getopt(argc, argv, "vis:f:c:e:g:")) != -1) {
        switch (option) {
            case 'v':
                show_results = true;
                break;
            case 'i':
                use_inner = true;
                break;
            case 'f':
                matrixASourceFile = optarg;
                break;
            case 'c':
                repl_fact = atoi(optarg);
                break;
            case 's':
                gen_seed = atoi(optarg);
                break;
            case 'e':
                exponent = atoi(optarg);
                break;
            case 'g':
                count_ge = true;
                ge_element = atof(optarg);
                break;
            default:
                fprintf(stderr, "error parsing argument %c exiting\n", option);
                MPI_Finalize();
                return 3;
        }
    }

    /* -------------------------- *
     *   Multiplication process   *
     * -------------------------- */
    GenericMultiplicationAlgorithm * algorithm;

    /* Select appropriate algorithm of parallel multiplication */
//    if (use_inner) { algorithm = new InnerABCAlgorithm(repl_fact); }
//    else           { algorithm = new ColAAlgorithm    (repl_fact); }
    algorithm = new ColAAlgorithm    (repl_fact);

    if ((gen_seed == -1) || (algorithm->isCoordinator() && matrixASourceFile.size() == 0)) {
        fprintf(stderr, "error: missing seed or sparse matrix file; exiting\n");
        // this call is not needed anymore, Boost's mpi::environment
        // class destructor will call MPI_Finalize():
        // MPI_Finalize();
        return 3;
    }

//    /* Playground */ {
//        cout << world.rank() << endl;
//        world.barrier();
//
//        DenseMatrixFragment *matrix = new DenseMatrixFragment();
//        if (world.rank() == 0) {
//            MatrixFragment::MatrixFragmentDescriptor size;
//            size.matrixWidth(3);
//            size.matrixHeight(3);
//            size.fragmentHeight(3);
//            size.fragmentWidth(3);
//            size.pRow(0); size.pCol(0);
//
//            DenseMatrixFragment matrix2(size);
//
//            matrix2 = 1.0;
//            matrix2.at(1,1) = 42.0;
//
//            mpi::broadcast(world, matrix2, 0);
//            *matrix = matrix2;
//        } else {
//            mpi::broadcast(world, *matrix, 0);
//        }
//        world.barrier();
//
//        cout << world.rank() << ": " << matrix->at(1, 1) << endl;
//        return 0;
//    }

    /* Load matrix from file */ {
        algorithm->step1_loadMatrixA(matrixASourceFile);
    }

    world.barrier();
    comm_start = MPI_Wtime();

    /* Prepare initial distribution of matrices */ {
        algorithm->step2_distributeMatrixA();
        algorithm->step3_generateMatrixB(gen_seed);
        // algorithm->step8_countAndPrintGe(ge_element);
        return 0;
    }

    world.barrier();
    comm_end = MPI_Wtime();
    comp_start = MPI_Wtime();

    /* Perform multiplication */ {
        algorithm->step4_redistributeMatrixA();
        algorithm->step5_redistributeMatrixB();

        for (int iter = 0; iter < exponent; ++iter) {
            algorithm->step6_performSingleMultiplication();
            if (iter < exponent - 1)
                algorithm->step7_setResultAsNewBMatrix();
        }
    }

    world.barrier();
    comp_end = MPI_Wtime();

    /* Print results */ {
        if (count_ge) {
            algorithm->step8_countAndPrintGe(ge_element);
        }
        if (show_results) {
            algorithm->step9_printResultMatrix();
        }
        if (algorithm->isCoordinator()) {
            cerr << "Runime statistics:\n"
            << "\tcommunication:\t" << (comm_end - comm_start) << "\n"
            << "\tcomputation:  \t" << (comp_end - comp_start) << endl;
        }
    }

    return 0;
}
