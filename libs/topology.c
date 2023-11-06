#include "topology.h"

void components(struct dinet_class *dinet, float p_site, long seed, unsigned long *components_size) {
	//
	// first pass to map nodes to boxes
	//
	unsigned long n=(*dinet).n_nodes;
	unsigned long *list = (unsigned long *)malloc(n*sizeof(long));
	unsigned long *component=(unsigned long *)malloc(n*sizeof(long));
	unsigned long i,j,k,l,first,last,o,r,n_components=0;
	for (i=0;i<n;i++) {
		component[i]=n;
		components_size[i]=0;
	}
	char *site = (char *)malloc(n*sizeof(char));
	for (i=0;i<n;i++) site[i]=(p_site==1 || ran2(&seed)<p_site)?1:0;
	for (o=0;o<n;o++) if (component[(*dinet).ordering[o]]==n) {
		i=(*dinet).ordering[o];
		if (site[i]==1) {
			component[i]=n_components;
			components_size[n_components]=1;
			first=0;
			last=1;
			list[first]=i;
			while (last>first) {
				j=list[first];
				first++;
				for (l=0;l<(*dinet).node[j].ou_degree;l++) {
					r=(*dinet).node[j].ou_arc[l];
					k=(*dinet).arc[r].succ;
					if (site[k]==1 && component[k]==n) {
						component[k]=n_components;
						components_size[n_components]++;
						list[last]=k;
						last++;
					}
				}
			}
			n_components++;
		}
	}
	free(site);
	free(list);
	free(component);
	return;
}

unsigned long one_step_renormalization(struct dinet_class *dinet, unsigned long box_size, struct dinet_class *dinet_b) {
	//
	// first pass to map nodes to boxes
	//
	unsigned long n=(*dinet).n_nodes;
	unsigned long m=(*dinet).n_arcs;
	unsigned long *list = (unsigned long *)malloc(n*sizeof(long));
	unsigned long *distance=(unsigned long *)malloc(n*sizeof(long));
	unsigned long *box=(unsigned long *)malloc(n*sizeof(long));
	unsigned long i,j,k,l,first,last,o,b,c,d,r,n_boxes,n_arcs;
	for (i=0;i<n;i++) distance[i]=n;
        n_boxes=0;
	for (o=0;o<n;o++) if (distance[(*dinet).ordering[o]]==n) {
		i=(*dinet).ordering[o];
		first=0;
		last=1;
		list[first]=i;
		distance[i]=0;
		while (last>first) {
			j=list[first];
			first++;
			box[j]=n_boxes;
			d=distance[j];
                        if (d+1<box_size) {
        	            	for (l=0;l<(*dinet).node[j].ou_degree;l++) {
					r=(*dinet).node[j].ou_arc[l];
					k=(*dinet).arc[r].succ;
					if (distance[k]==n) {
						distance[k]=d+1;
						list[last]=k;
						last++;
					}
				}
			}
		}
		n_boxes++;
	}
	if (dinet_b==NULL) return n_boxes;
	//
	// second pass to calculate box degrees
	//
	(*dinet_b).n_nodes = n_boxes;
	(*dinet_b).node=(struct dinode_class *)malloc(n_boxes*sizeof(struct dinode_class));
        for (b=0;b<n_boxes;b++) (*dinet_b).node[b].ou_degree=(*dinet_b).node[b].in_degree=0;
        n_arcs=0;
	for (r=0;r<m;r++) {
		i=(*dinet).arc[r].pred;
		j=(*dinet).arc[r].succ;
		b=box[i];
		c=box[j];
		if (b!=c) {
			(*dinet_b).node[b].ou_degree++;
			(*dinet_b).node[c].in_degree++;
			n_arcs++;
		}
	}
	//
	// third pass to connect the boxes
	//
	(*dinet_b).n_arcs=n_arcs;
        (*dinet).arc=(struct arc_class *)malloc(n_arcs*sizeof(struct arc_class));
	for (b=0;b<n_boxes;b++) {
		(*dinet).node[b].ou_arc=(unsigned long *)malloc((*dinet).node[b].ou_degree*sizeof(long));
		(*dinet).node[b].in_arc=(unsigned long *)malloc((*dinet).node[b].in_degree*sizeof(long));
		(*dinet_b).node[b].ou_degree=(*dinet_b).node[b].in_degree=0;
	}
	n_arcs=0;
	for (r=0;r<m;r++) {
		i=(*dinet).arc[r].pred;
		j=(*dinet).arc[r].succ;
		b=box[i];
		c=box[j];
		if (b!=c) {
			(*dinet).node[b].ou_arc[(*dinet_b).node[b].ou_degree]=n_arcs;
			(*dinet).node[c].in_arc[(*dinet_b).node[c].ou_degree]=n_arcs;
			(*dinet_b).node[b].ou_degree++;
			(*dinet_b).node[c].in_degree++;
			(*dinet).arc[n_arcs].pred=b;
			(*dinet).arc[n_arcs].succ=c;
			n_arcs++;
		}
	}
	free(list);
	free(distance);
	free(box);
	return n_boxes;
}

unsigned long average_number_of_nodes_by_distance(struct dinet_class *dinet, float *nodes_at_distance_array) {
	unsigned long n=(*dinet).n_nodes;
	unsigned long i,j,k,l,r,last,first,d;
	unsigned long *list = (unsigned long *)malloc(n*sizeof(long));
	unsigned long *distance = (unsigned long *)malloc(n*sizeof(long));
	unsigned long *visited = (unsigned long *)malloc(n*sizeof(long));
	unsigned long diameter=0;
        for (i=0;i<n;i++) {
		nodes_at_distance_array[i]=0;
		visited[i]=n;
	}
	unsigned long n_roots=0;
	for (i=0;i<n;i++) if ((*dinet).node[i].in_degree==0) {
		n_roots++;
		first=0;
		last=1;
		list[first]=i;
		distance[i]=0;
		visited[i]=i;
		while (last>first) {
			j=list[first];
			first++;
			d=distance[j];
			nodes_at_distance_array[d]++;
			if (d>diameter) diameter=d;
                    	for (l=0;l<(*dinet).node[j].ou_degree;l++) {
				r=(*dinet).node[j].ou_arc[l];
				k=(*dinet).arc[r].succ;
				if (visited[k]!=i) {
					visited[k]=i;
					distance[k]=d+1;
					list[last]=k;
					last++;
				}
			}
		}
	}
        for (i=0;i<n;i++) nodes_at_distance_array[i]/=n_roots;
	free(list);
	free(distance);
	free(visited);
	return diameter;
}

void topology_ordering_topological(struct dinet_class *dinet) {
	printf("topology_ordering_topological start...\n");
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
	printf("topology_ordering_topological end.\n");
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


/********************************************************************************************
	methods to estimate the q index
********************************************************************************************/

double likelihood(unsigned long n, unsigned long *degree, unsigned long kmax, float q_inv) {
	double sum=0.0;
	unsigned long k,i;
	for (k=1;k<kmax;k++) sum+=exp( lgamma(k) - lgamma(k+q_inv) );
	sum=(n*log(sum));
	for (i=0;i<n;i++) if (degree[i]>0) sum-=lgamma(degree[i])-lgamma(degree[i]+q_inv);
	return sum;
}

#define R 0.61803399
#define TOLERANCE 1.0e-06
float q_max_likelihod(unsigned long n, unsigned long *degree, unsigned long kmax) {
	printf("q_max_likelihod start...\n");
	// Based on "Routine for Golden Section Search", Numerical Recipes in C
	float f1,f2,x0,x1,x2,x3;
	float ax=1.0;
	float bx=2.0;
	float cx=3.0;
	f1=likelihood(n,degree,kmax,bx);
	f2=likelihood(n,degree,kmax,cx);
	while (f2<f1) {
		bx++;
		cx++;
		f1=likelihood(n,degree,kmax,bx);
		f2=likelihood(n,degree,kmax,cx);
		//printf("\t%f,%f,%f,%f",bx,cx,f1,f2); getchar();
	}
	//printf("\tright bound found\n");
	x0=ax;
	x3=cx;
	if (fabs(cx-bx) > fabs(bx-ax)) {
		x1=bx;
		x2=bx+(1.0-R)*(cx-bx);
	}
	else {
		x2=bx;
		x1=bx-(1.0-R)*(bx-ax);
	}
	f1=likelihood(n,degree,kmax,x1);
	f2=likelihood(n,degree,kmax,x2);
	while ( fabs(x3-x0) > TOLERANCE*(fabs(x1)+fabs(x2)) ) {
		if (f2 < f1) {
			x0=x1;
			x1=x2;
			x2=R*x1+(1.0-R)*x3;
			f1=f2;
			f2=likelihood(n,degree,kmax,x2);
		}
		else {
			x3=x2;
			x2=x1;
			x1=R*x2+(1.0-R)*x0;
			f2=f1;
			f1=likelihood(n,degree,kmax,x1);
		}
	}
	printf("q_max_likelihod end.\n");
	if (f1 < f2) return 1.0/x1;
	else return 1.0/x2;
}
#undef R
#undef TOLERANCE

void q_max_likelihod_schedule(struct dinet_class *dinet) {
	unsigned long i;
	unsigned long *degree=(unsigned long *)malloc((*dinet).n_nodes*sizeof(long));
	printf("q_ou...\n");
	for (i=0;i<(*dinet).n_nodes;i++) degree[i]=(*dinet).node[i].ou_degree;
	(*dinet).q_ou = q_max_likelihod((*dinet).n_nodes, degree, (*dinet).n_nodes);
	printf("q_in...\n");
	for (i=0;i<(*dinet).n_nodes;i++) degree[i]=(*dinet).node[i].in_degree;
	(*dinet).q_in = q_max_likelihod((*dinet).n_nodes, degree, (*dinet).n_nodes);
}
