#ifndef _RED_H_
#define _RED_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <strings.h>

// // flags

// // classes

struct dinode_class {
	unsigned long in_degree;
	unsigned long ou_degree;
	unsigned long *in_arc;
	unsigned long *ou_arc;
	unsigned long ordering;
	unsigned long generation;
        double total_float;
        double duration;
	double value0;
	double value;
};

struct arc_class {
	unsigned long pred;
	unsigned long succ;
	double value;
};

struct dinet_class {
	char name[500];
	unsigned long n_nodes;
	unsigned long n_arcs;
	float q_ou;
	float q_in;
	struct dinode_class *node;
	struct arc_class *arc;
	unsigned long *ordering;
};

// // functions

extern int compare_float(const void * a, const void * b);
extern int compare_double(const void * a, const void * b);
extern void free_dinet(struct dinet_class *dinet);

#endif
