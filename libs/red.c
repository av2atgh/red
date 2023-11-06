#include "red.h"

// functions

int compare_float(const void * a, const void * b) {
	if ( *(float*)a == *(float*)b ) return 0;
	else {
		if ( *(float*)a <  *(float*)b ) return -1;
		else return 1;
	}
}

int compare_double(const void * a, const void * b) {
	if ( *(double*)a == *(double*)b ) return 0;
	else {
		if ( *(double*)a <  *(double*)b ) return -1;
		else return 1;
	}
}

void free_dinet(struct dinet_class *dinet) {
	unsigned long i,n=(*dinet).n_nodes;
	for (i=0;i<n;i++) {
		free((*dinet).node[i].ou_arc);
		free((*dinet).node[i].in_arc);
	}
	free((*dinet).node);
	free((*dinet).arc);
	free((*dinet).ordering);
	return;
}

