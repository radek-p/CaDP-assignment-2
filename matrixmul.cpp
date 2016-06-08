#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <getopt.h>
#include <string>
#include <iostream>
#include <fstream>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedef"
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <boost/mpi.hpp>
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
    shared_ptr<GenericMultiplicationAlgorithm> algorithm;

    /* Select appropriate algorithm of parallel multiplication */
    if (use_inner) { algorithm = make_shared<InnerABCAlgorithm>(repl_fact); }
    else           { algorithm = make_shared<ColAAlgorithm>    (repl_fact); }

    if (exponent < 1) {
        if (algorithm->isCoordinator())
            cerr << "Error: Invalid exponent '" << exponent << "'." << endl
                 << "Exponent must be greater than 0." << endl;
        return 3;
    }

    if (gen_seed == -1) {
        if (algorithm->isCoordinator())
            cerr << "Error: Missing seed." << endl;
        return 3;
    }

    /* Load matrix from file */ {
        bool success = algorithm->step1_loadMatrixA(matrixASourceFile);
        if (!success) {
            if (algorithm->isCoordinator())
                cerr << "Error: Cannot read matrix A file:" << endl
                     << "'"  << matrixASourceFile << "'" << endl;
            return 3;
        }
    }

    world.barrier();
    comm_start = MPI_Wtime();

    /* Prepare initial distribution of matrices */ {
        algorithm->step2_distributeMatrixA();
        algorithm->step3_generateMatrixB(gen_seed);
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
            cerr << "\n\nRunime statistics:\n"
            << "\tinitial distribution phase:\t" << (comm_end - comm_start) << "\n"
            << "\t product computation phase:\t" << (comp_end - comp_start) << endl;
        }
    }

    return 0;
}
