/*
 * Matrix generation library for the MPI assignment.
 * Concurrent and Distributed Programming Course, spring 2016.
 * Faculty of Mathematics, Informatics and Mechanics.
 * University of Warsaw, Warsaw, Poland.
 *
 * Copyright (C) Krzysztof Rzadca, 2016
 */

#ifndef __MIMUW_MATGEN_H__
#define __MIMUW_MATGEN_H__

#ifdef __cplusplus
extern "C"
{
#endif


/**
 * Generates a single entry at row x col
 * Stateless generator
 * @param seed seed for the generator (some seeds switch op mode)
 * @param row row coordinate of the generated element
 * @param col col coordinate of the generated element
 * @return a very-pseudo random matrix element
 */
double generate_double(int seed, int row, int col);



#ifdef __cplusplus
}
#endif


#endif /* __MIMUW_MATGEN_H__ */
