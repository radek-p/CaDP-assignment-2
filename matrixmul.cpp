#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <getopt.h>
#include <string>
#include <iostream>

#include "algorithms/GenericMultiplicationAlgorithm.h"
#include "algorithms/ColAAlgorithm.h"
#include "algorithms/InnerABCAlgorithm.h"
#include "tests/testRunner.h"

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

    MPI_Init(&argc, &argv);

    // TODO REMOVE FROM HERE
    int res = runTests();

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
    if (use_inner) { algorithm = new InnerABCAlgorithm(matrixASourceFile, gen_seed, exponent, repl_fact); }
    else           { algorithm = new ColAAlgorithm    (matrixASourceFile, gen_seed, exponent, repl_fact); }

    if ((gen_seed == -1) || (algorithm->isCoordinator() && matrixASourceFile.size() == 0)) {
        fprintf(stderr, "error: missing seed or sparse matrix file; exiting\n");
        MPI_Finalize();
        return 3;
    }

    /* Step 1. - load and distribute matrices */ {
        comm_start = MPI_Wtime();
        algorithm->step1_prepareData();
        MPI_Barrier(MPI_COMM_WORLD);
        comm_end = MPI_Wtime();
    }
    /* Step 2. - do required multiplications */ {
        comp_start = MPI_Wtime();
        algorithm->step2_performMultiplication();
        MPI_Barrier(MPI_COMM_WORLD);
        comp_end = MPI_Wtime();
    }
    /* Step 3. - print appropriate results */ {
        algorithm->step3_printResults(show_results, count_ge, ge_element);
        if (algorithm->isCoordinator()) {
            cerr << "Runime statistics:\n"
            << "\tcommunication:\t" << (comm_end - comm_start) << "\n"
            << "\tcomputation:  \t" << (comp_end - comp_start) << endl;
        }
    }

    MPI_Finalize();

    // TODO REMOVE FROM HERE
    return res;
}
