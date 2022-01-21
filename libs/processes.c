#include "processes.h"

void processes_scheduling(struct dinet_class *dinet, const char *load_distribution, float p1, float p2, long seed0) {
	unsigned long i,j,l,r,o;
	unsigned long n=(*dinet).n_nodes;
	unsigned long m=(*dinet).n_arcs;
	long seed=seed0;

	// // assign durations

	if (strcmp(load_distribution,"exponential") {
		float mu=p1;
		for (i=0;i<n;i++) (*dinet).node[i].duration=1+(unsigned long)(-(mu*log(ran2(&seed))));
	}
	else if (strcmp(load_distribution,"lognormal") {
		float mu=p1;
		float sigma=p2;
		for (i=0;i<n;i++) (*dinet).node[i].duration=1+(unsigned long)exp(mu+sigma*rand_normal(&seed));
	}
	else {
		printf("warning,distribution not found,exit\n");
		exit(1);
	}

	// // forward pass

	for (i=0;i<n;i++) (*dinet).node[i].start_early=0;

	for (o=0;o<n;o++) {
		i=(*dinet).ordering[o];
		(*dinet).node[i].finish_early=(*dinet).node[i].start_early+(*dinet).node[i].duration;
		for (l=0;l<(*dinet).node[i].ou_degree;l++) {
			r=(*dinet).node[i].ou_arc[l];
			j=(*dinet).arc[r].succ;
			if ((*dinet).node[i].finish_early>(*dinet).node[j].start_early)
				(*dinet).node[j].start_early=(*dinet).node[i].finish_early;
		}
	}

	// // backward pass

	float finish=0.0;
	for (i=0;i<n;i++) if ((*dinet).node[i].finish_early>finish) finish=(*dinet).node[i].finish_early;
	for (i=0;i<n;i++) (*dinet).node[i].finish_late=finish;

	for (o=0;o<n;o++) {
		i=(*dinet).ordering[n-1-o];
		(*dinet).node[i].start_late=(*dinet).node[i].finish_late-(*dinet).node[i].duration;
		for (l=0;l<(*dinet).node[i].in_degree;l++) {
			r=(*dinet).node[i].in_arc[l];
			j=(*dinet).arc[r].pred;
			if ((*dinet).node[i].start_late<(*dinet).node[j].finish_late)
				(*dinet).node[j].finish_late=(*dinet).node[i].start_late;
		}
	}

	// // free floats

	for (r=0;r<m;r++) {
		i=(*dinet).arc[r].pred;
		j=(*dinet).arc[r].succ;
		if ((*dinet).node[j].start_late<(*dinet).node[i].finish_early) {
			printf("warning,free_float error,\n");
			exit(1);
		}
		(*dinet).arc[r].free_float=(*dinet).node[j].start_late-(*dinet).node[i].finish_early;
	}

	// // total floats 

	for (i=0;i<n;i++) (*dinet).node[i].total_float=(*dinet).node[i].finish_late-(*dinet).node[i].finish_early;

	return;
}
