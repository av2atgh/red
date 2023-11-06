#include "generators.h"

void generators_duplication_split(struct dinet_class *dinet, unsigned long n_nodes, float duplication_rate, long seed0) {
	printf("function,generators_duplication_split,start\n");
	unsigned long n_arcs=1;
	float dup=duplication_rate;
	long seed=seed0;

	// initialize

	(*dinet).node=(struct dinode_class *)malloc(n_nodes*sizeof(struct dinode_class));
	(*dinet).arc=(struct arc_class *)malloc(n_arcs*sizeof(struct arc_class));

	(*dinet).node[0].in_degree=0;
	(*dinet).node[0].ou_degree=1;
	(*dinet).node[0].in_arc=(unsigned long *)malloc((*dinet).node[0].in_degree*sizeof(long));
	(*dinet).node[0].ou_arc=(unsigned long *)malloc((*dinet).node[0].ou_degree*sizeof(long));
	(*dinet).node[0].ou_arc[0]=0;

	(*dinet).node[1].in_degree=1;
	(*dinet).node[1].ou_degree=0;
	(*dinet).node[1].in_arc=(unsigned long *)malloc((*dinet).node[1].in_degree*sizeof(long));
	(*dinet).node[1].ou_arc=(unsigned long *)malloc((*dinet).node[1].ou_degree*sizeof(long));
	(*dinet).node[1].in_arc[0]=0;

	(*dinet).arc[0].pred=0;
	(*dinet).arc[0].succ=1;

	// grow

	unsigned long i,j,k,l,r,delta_r;

	for (i=2;i<n_nodes;i++) {
		j=i*ran2(&seed);
		if (ran2(&seed)<dup) {
			// duplication
			(*dinet).node[i].in_degree=(*dinet).node[j].in_degree;
			(*dinet).node[i].ou_degree=(*dinet).node[j].ou_degree;
			(*dinet).node[i].in_arc=(unsigned long *)malloc((*dinet).node[i].in_degree*sizeof(long));
			(*dinet).node[i].ou_arc=(unsigned long *)malloc((*dinet).node[i].ou_degree*sizeof(long));
			delta_r=n_arcs+(*dinet).node[j].in_degree+(*dinet).node[j].ou_degree;
			(*dinet).arc=(struct arc_class *)realloc((*dinet).arc,delta_r*sizeof(struct arc_class));
			for (l=0;l<(*dinet).node[j].in_degree;l++) {
				r=(*dinet).node[j].in_arc[l];
				k=(*dinet).arc[r].pred;
				(*dinet).node[i].in_arc[l]=n_arcs;
				(*dinet).node[k].ou_degree++;
				(*dinet).node[k].ou_arc=(unsigned long *)realloc((*dinet).node[k].ou_arc,(*dinet).node[k].ou_degree*sizeof(long));
				(*dinet).node[k].ou_arc[(*dinet).node[k].ou_degree-1]=n_arcs;
				(*dinet).arc[n_arcs].pred=k;
				(*dinet).arc[n_arcs].succ=i;
				n_arcs++;
			}
			for (l=0;l<(*dinet).node[j].ou_degree;l++) {
				r=(*dinet).node[j].ou_arc[l];
				k=(*dinet).arc[r].succ;
				(*dinet).node[i].ou_arc[l]=n_arcs;
				(*dinet).node[k].in_degree++;
				(*dinet).node[k].in_arc=(unsigned long *)realloc((*dinet).node[k].in_arc,(*dinet).node[k].in_degree*sizeof(long));
				(*dinet).node[k].in_arc[(*dinet).node[k].in_degree-1]=n_arcs;
				(*dinet).arc[n_arcs].pred=i;
				(*dinet).arc[n_arcs].succ=k;
				n_arcs++;
			}
		}
		else {
			// split
			(*dinet).node[i].in_degree=1;
			(*dinet).node[i].ou_degree=(*dinet).node[j].ou_degree;
			(*dinet).node[i].in_arc=(unsigned long *)malloc((*dinet).node[i].in_degree*sizeof(long));
			(*dinet).node[i].ou_arc=(unsigned long *)malloc((*dinet).node[i].ou_degree*sizeof(long));
			(*dinet).node[i].in_arc[0]=n_arcs;
			for (l=0;l<(*dinet).node[j].ou_degree;l++) {
				r=(*dinet).node[j].ou_arc[l];
				(*dinet).node[i].ou_arc[l]=r;
				(*dinet).arc[r].pred=i;
			}
			(*dinet).node[j].ou_degree=1;
			(*dinet).node[j].ou_arc=(unsigned long *)realloc((*dinet).node[j].ou_arc,(*dinet).node[j].ou_degree*sizeof(long));
			(*dinet).node[j].ou_arc[0]=n_arcs;
			delta_r=n_arcs+1;
			(*dinet).arc=(struct arc_class *)realloc((*dinet).arc,delta_r*sizeof(struct arc_class));
			(*dinet).arc[n_arcs].pred=j;
			(*dinet).arc[n_arcs].succ=i;
			n_arcs++;
		}
	}

	(*dinet).n_nodes=n_nodes;
	(*dinet).n_arcs=n_arcs;

	sprintf((*dinet).name,"generators_duplication_split.dup_%f.n_nodes_%lu.seed0_%lu",
		duplication_rate,
		n_nodes,
		(unsigned long)(-seed0)
	);


	printf("function,generators_duplication_split,end\n");
	return;
}
