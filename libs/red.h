#ifndef _RED_H_
#define _RED_H_

#include <stdlib.h>

// // flags

// // classes

struct dinode_class {
	unsigned long in_degree;
	unsigned long ou_degree;
	unsigned long *in_arc;
	unsigned long *ou_arc;
	unsigned long ordering;
	unsigned long generation;
	unsigned long duration;
	unsigned long start_early;
	unsigned long start_late;
	unsigned long finish_early;
	unsigned long finish_late;
	unsigned long total_float;
	unsigned long non_critical_path_length;
	unsigned long in_list;
};

struct arc_class {
	unsigned long pred;
	unsigned long succ;
	unsigned long free_float;
};

struct dinet_class {
	char name[500];
	unsigned long n_nodes;
	unsigned long n_arcs;
	struct dinode_class *node;
	struct arc_class *arc;
	unsigned long *ordering;
	unsigned long kmax;
	float q_in;
	float q_ou;
	unsigned long *list;
};


// // functions


extern void free_dinet(struct dinet_class *dinet);

#endif
