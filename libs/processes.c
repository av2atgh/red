#include "processes.h"

float processes_node_values(struct dinet_class *dinet, char *load_distribution, float p1, float p2, long seed0) {
	printf("function,processes_node_values,start\n");
	long seed=seed0;
	unsigned long i,n=(*dinet).n_nodes;
	if (strcmp(load_distribution,"exponential")==0) {
		float mu=p1;
		for (i=0;i<n;i++) (*dinet).node[i].value0=-(mu*log(ran2(&seed)));
	}
	else if (strcmp(load_distribution,"lognormal")==0) {
		float mu=p1;
		float sigma=p2;
		for (i=0;i<n;i++) (*dinet).node[i].value0=exp(mu+sigma*rand_normal(&seed));
	}
	else {
		printf("warning,distribution not found,exit\n");
		exit(1);
	}
	float max=0.0;
	for (i=0;i<n;i++) if ((*dinet).node[i].value0>max) max=(*dinet).node[i].value0;
	printf("function,processes_node_values,end\n");
	return max;
}

void processes_arc_values(struct dinet_class *dinet, char *load_distribution, float p1, float p2, long seed0, const char *mode) {
	printf("function,processes_arc_values,start\n");
	long seed=seed0;
	unsigned long i,j,l,r,o,n=(*dinet).n_nodes,m=(*dinet).n_arcs;
	printf("report,mode,%s\n",mode);
	if (strcmp(mode,"scheduling")==0) {
		// assign node durations
		if (strcmp(load_distribution,"zero")==0) {
			for (i=0;i<n;i++) (*dinet).node[i].value=(*dinet).node[i].value0=0.0;
		}
		else if (strcmp(load_distribution,"exponential")==0) {
			float mu=p1;
			for (i=0;i<n;i++) (*dinet).node[i].value0=-(mu*log(ran2(&seed)));
		}
		else if (strcmp(load_distribution,"lognormal")==0) {
			float mu=p1;
			float sigma=p2;
			for (i=0;i<n;i++) (*dinet).node[i].value0=exp(mu+sigma*rand_normal(&seed));
		}
		else if (strcmp(load_distribution,"given")==0) {
			for (i=0;i<n;i++) (*dinet).node[i].value0=(*dinet).node[i].duration;
			printf("info, using given durations\n");
		}
		else {
			printf("warning,distribution not found,exit\n");
			exit(1);
		}
		// forward pass to assign early finish
		for (i=0;i<n;i++) (*dinet).node[i].value=(*dinet).node[i].value0;
		float pass;
		for (o=0;o<n;o++) {
			i=(*dinet).ordering[o];
			for (l=0;l<(*dinet).node[i].ou_degree;l++) {
				r=(*dinet).node[i].ou_arc[l];
				j=(*dinet).arc[r].succ;
				pass=(*dinet).node[i].value+(*dinet).node[j].value0;
				if (pass>(*dinet).node[j].value) (*dinet).node[j].value=pass;
			}
		}
		// backward pass to assign late finish
		float max=0.0;
		for (i=0;i<n;i++) if ((*dinet).node[i].value>max) max=(*dinet).node[i].value;
		float *late=(double *)malloc(n*sizeof(double));
		for (i=0;i<n;i++) late[i]=max;
		for (o=0;o<n;o++) {
			i=(*dinet).ordering[n-1-o];
			for (l=0;l<(*dinet).node[i].in_degree;l++) {
				r=(*dinet).node[i].in_arc[l];
				j=(*dinet).arc[r].pred;
				pass=(*dinet).node[i].value-(*dinet).node[i].value0;
				if (pass<late[j]) late[j]=pass;
			}
		}
                // free floats on edges
		for (r=0;r<m;r++) {
			i=(*dinet).arc[r].pred;
			j=(*dinet).arc[r].succ;
			(*dinet).arc[r].value=late[j]-(*dinet).node[i].value;
		}
                // total float on nodes
                for (i=0;i<n;i++) {
			(*dinet).node[i].total_float=late[i]-(*dinet).node[i].value; 
		}
		free(late);
	}
	else {
		if (strcmp(load_distribution,"zero")==0) {
			for (i=0;i<m;i++) (*dinet).arc[i].value=0.0;
		}
		else if (strcmp(load_distribution,"exponential")==0) {
			float mu=p1;
			for (i=0;i<m;i++) (*dinet).arc[i].value=-(mu*log(ran2(&seed)));
		}
		else if (strcmp(load_distribution,"lognormal")==0) {
			float mu=p1;
			float sigma=p2;
			for (i=0;i<m;i++) (*dinet).arc[i].value=exp(mu+sigma*rand_normal(&seed));
		}
		else {
			printf("warning,distribution not found,exit\n");
			exit(1);
		}
	}
	printf("function,processes_arc_values,end\n");
	return;
}

#define SUMSUM 0
#define MAXSUM 1
#define MAXMAX 2
void processes_summax(struct dinet_class *dinet, char *use_rule) {
	printf("function,process_summax,start\n");
	char rule;
	if      (strcmp(use_rule,"sumsum")==0) rule=SUMSUM;
	else if (strcmp(use_rule,"maxsum")==0) rule=MAXSUM;
	else if (strcmp(use_rule,"maxmax")==0) rule=MAXMAX;
	else {
		printf("warning,rule %s not in use,exit\n",use_rule);
		exit(1);
	}
	float pass;
	unsigned long o,i,j,l,r,n=(*dinet).n_nodes;
	for (i=0;i<n;i++) (*dinet).node[i].value=(*dinet).node[i].value0;
	for (o=0;o<n;o++) {
		i=(*dinet).ordering[o];
		for (l=0;l<(*dinet).node[i].ou_degree;l++) {
			r=(*dinet).node[i].ou_arc[l];
			j=(*dinet).arc[r].succ;
			pass=(*dinet).node[i].value-(*dinet).arc[r].value;
			if (pass>0) switch (rule) {
				case SUMSUM:
					(*dinet).node[j].value+=pass/(*dinet).node[j].in_degree;
					break;
				case MAXSUM:
					pass+=(*dinet).node[j].value0;
					if (pass>(*dinet).node[j].value) (*dinet).node[j].value=pass;
					break;
				case MAXMAX:
					if (pass>(*dinet).node[j].value) (*dinet).node[j].value=pass;
					break;
			}
		}
	}
	printf("function,process_summax,end\n");
	return;
}
#undef SUMSUM
#undef MAXSUM
#undef MAXMAX

float processes_ouput_max_from_out_degree_0(struct dinet_class *dinet) {
	printf("function,processes_ouput_max_from_out_degree_0,start\n");
	unsigned long i,n=(*dinet).n_nodes;
	float max=0.0;
	for (i=0;i<n;i++) if ((*dinet).node[i].ou_degree==0 && (*dinet).node[i].value>max) max=(*dinet).node[i].value;
	printf("function,processes_ouput_max_from_out_degree_0,end\n");
	return max;
} 
	
