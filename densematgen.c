/*
 * An implementation of the matrix generation library
 * for the MPI assignment.
 * Concurrent and Distributed Programming Course, spring 2016.
 * Faculty of Mathematics, Informatics and Mechanics.
 * University of Warsaw, Warsaw, Poland.
 *
 * The implementation is very, very loosely based on Xorshift RNGs, 
 * but doesn't keep the state, so it's not really pseudo-random.
 * 
 * The adaptation is copyright (C) Krzysztof Rzadca, 2016.
 */

#include <stdint.h>
#include <stdlib.h>

#include "densematgen.h"

uint32_t naive_xorshift(uint32_t x, uint32_t y, uint32_t w) 
{
  x ^= x << 11;
  y ^= y << 7;
  x ^= y;
  w ^= w << 19;
  w ^= x;
  return w;
}

double generate_double(int seed, int row, int col)
{
  if (seed == 0)
    return 0;
  if (seed == 1)
    return 1;
  if (seed == 2)
    return (row==col)? 1:0;
  if (seed == 3)
    return row*10+col;
  if (seed > 10)
  {
    uint32_t resolution = 1000;
    uint32_t rand_32 = naive_xorshift((uint32_t) seed, (uint32_t) row, (uint32_t) col) % resolution;
    double rand = (rand_32) / ((double) resolution);
    return (1.0 - 0.0) * rand;
  }
  return -1;
}
