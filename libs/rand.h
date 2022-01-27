#ifndef _RAND_H_
#define _RAND_H_

#include <math.h>

extern float ran2(long *idum);
extern float rand_normal(long *idum);
extern float rand_gamma(float alpha, long *idum);
extern float rand_beta(float alpha, float beta, long *idum);

#endif
