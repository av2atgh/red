#ifndef _RAND_H_
#define _RAND_H_

#include <math.h>

float ran2(long *idum);
float rand_normal(long *idum);
float rand_gamma(float alpha, long *idum);
float rand_beta(float alpha, float beta, long *idum);

#endif
