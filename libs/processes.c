#include "auxo.h"

void auxo_sample(struct dinet_class *dinet, struct parameter_class parameters) {
	unsigned long n_nodes=parameters.n_nodes;
	unsigned long n_arcs=1;
	float dup=parameters.dup;
	long seed=parameters.seed;

	// initialize

	(*dinet).node=(struct dinode_class *)malloc(n_nodes*sizeof(struct dinode_class));
	(*dinet).arc=(struct arc_class *)malloc(n_arcs*sizeof(struct arc_class));
	(*dinet).list=(unsigned long *)malloc(2*n_nodes*sizeof(long));

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

	sprintf((*dinet).filename,"auxo.one_sample.dup_%.2f.n_nodes_%lu",
		parameters.dup,
		parameters.n_nodes
	);


	return;

}

void auxo_ordering_topological(struct dinet_class *dinet) {
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

void auxo_generation(struct dinet_class *dinet) {
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

void auxo_scheduling(struct dinet_class *dinet, struct distribution_parameters dispar) {
	unsigned long i,j,l,r,o;
	unsigned long n=(*dinet).n_nodes;
	unsigned long m=(*dinet).n_arcs;
	long seed=dispar.seed;

	// // assign durations

	if (dispar.distribution==LOGNORMAL)
		for (i=0;i<n;i++) (*dinet).node[i].duration=1+(unsigned long)exp(dispar.mu+dispar.sigma*rand_normal(&seed));
	// exponential is default
	for (i=0;i<n;i++) (*dinet).node[i].duration=1+(unsigned long)(-(dispar.mu*log(ran2(&seed))));

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

void auxo_free(struct dinet_class *dinet) {
	free((*dinet).node);
	free((*dinet).arc);
	free((*dinet).ordering);
	free((*dinet).list);
	return;
}

void auxo_write_adjlist(struct dinet_class *dinet) {
	printf("write_adjlist\n");
	char filename[500];
	sprintf(filename,"%s.adjlist.txt",(*dinet).filename);
	printf("\t%s out\n",filename);
	FILE *file=fopen(filename,"wt");
	unsigned long i,j,l,r;
	for (i=0;i<(*dinet).n_nodes;i++) {
		fprintf(file,"%lu",i);
		for (l=0;l<(*dinet).node[i].ou_degree;l++) {
			r=(*dinet).node[i].ou_arc[l];
			j=(*dinet).arc[r].succ;
			fprintf(file," %lu",j);
		}
		fprintf(file,"\n");
	}
	fclose(file);
}

void auxo_in_degree_histogram_log_binning(struct dinet_class *dinet) {
	unsigned long i,*x;
	x=(unsigned long *)malloc((*dinet).n_nodes*sizeof(long));
	for (i=0;i<(*dinet).n_nodes;i++) x[i]=(*dinet).node[i].in_degree;
	histogram_log_binning((*dinet).filename,".in_degree",0,(*dinet).n_nodes,x);
	free(x);
	return;
}

void auxo_ou_degree_histogram_log_binning(struct dinet_class *dinet) {
	unsigned long i,*x;
	x=(unsigned long *)malloc((*dinet).n_nodes*sizeof(long));
	for (i=0;i<(*dinet).n_nodes;i++) x[i]=(*dinet).node[i].ou_degree;
	histogram_log_binning((*dinet).filename,".ou_degree",0,(*dinet).n_nodes,x);
	free(x);
	return;
}

void auxo_dinet_q_max_likelihod(struct dinet_class *dinet) {
	unsigned long i,kmax,n=(*dinet).n_nodes;
	unsigned long *degree=(unsigned long *)malloc(n*sizeof(long));
	int d;
	for (d=0;d<2;d++) {
		switch (d) {
			case 0: for (i=0;i<n;i++) degree[i]=(*dinet).node[i].in_degree; break;
			case 1: for (i=0;i<n;i++) degree[i]=(*dinet).node[i].ou_degree; break;
		}
		kmax=degree[0];
		for (i=1;i<n;i++) if (degree[i]>kmax) kmax=degree[i];
		kmax=pow(2.0,log(kmax)/log(2.0)+1);
		switch (d) {
			case 0: (*dinet).q_in=q_max_likelihod(n,degree,kmax); break;
			case 1: (*dinet).q_ou=q_max_likelihod(n,degree,kmax); break;
		}
	}
	free(degree);
	return;
}
