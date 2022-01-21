#include "topology.h"

void topology_ordering_topological(struct dinet_class *dinet) {
	unsigned long n=(*dinet).n_nodes;
	unsigned long m=(*dinet).n_arcs;
	unsigned long i,j,l,r,last=0,n_ordered=0;
	unsigned long *list=(unsigned long *)malloc(n*sizeof(long));
	(*dinet).ordering=(unsigned long *)malloc(n*sizeof(long));
	unsigned long *in_degree=(unsigned long *)malloc(n*sizeof(long));
	for (i=0;i<n;i++) in_degree[i]=(*dinet).node[i].in_degree;
	// set all arcs to not visited
	bool *visited=(bool *)malloc(m*sizeof(bool)); 
	for (r=0;r<m;r++) visited[r]=false;
	// start from nodes with in-degree 0
	for (i=0;i<n;i++) if ((*dinet).node[i].in_degree==0) {
		list[last]=i; last++;
	}
	// ordering loop
	while (last>0) {
		last--; i=list[last];
		(*dinet).ordering[n_ordered]=i;
		(*dinet).node[i].ordering=n_ordered;
		n_ordered++;
		for (l=0;l<(*dinet).node[i].ou_degree;l++) {
			r=(*dinet).node[i].ou_arc[l];
			if (!visited[r]) {
				visited[r]=true;
				j=(*dinet).arc[r].succ;
				in_degree[j]--;
				if (in_degree[j]==0) {list[last]=j; last++;}
			}
		}
	}
	// check if all arcs were visited
	r=0;
	while (r<m && visited[r]) r++;
	if (r<m) printf("\tWARNING, schedule is not a DAG\n");
	free(list);
	free(in_degree);
	free(visited);
	return;
}

void topology_generation(struct dinet_class *dinet) {
	unsigned long i,j,l,r,o;
	unsigned long n=(*dinet).n_nodes;
	for (i=0;i<n;i++) (*dinet).node[i].generation=0;
	for (o=0;o<n-1;o++) {
		i=(*dinet).ordering[o];
		for (l=0;l<(*dinet).node[i].ou_degree;l++) {
			r=(*dinet).node[i].ou_arc[l];
			j=(*dinet).arc[r].succ;
			if ((*dinet).node[j].generation<=(*dinet).node[i].generation)
				(*dinet).node[j].generation=(*dinet).node[i].generation+1;
		}
	}
	return;
}
