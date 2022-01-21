#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void statistics_histogram_log_binning(const char *filename, const char *extension, unsigned long first, unsigned long last, unsigned long *x);
float statistics_power_law_exponent_max_likelihod(unsigned long n, unsigned long *x, unsigned long xmax);

#endif
