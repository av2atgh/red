#include "red.h"
#include "topology.h"
#include "generators.h"
#include "processes.h"
#include <float.h>
#include <string.h>

float variance(unsigned long n, double *x) {
	double sx=0.0,sxx=0.0,dx;
	unsigned long i;
	for (i=0;i<n;i++) sx+=x[i];
	sx/=n;
	for (i=0;i<n;i++) {
		dx=x[i]-sx;
		sxx+=dx*dx;
	}
	sxx/=n;
	return (float)sxx;
}

float slope(unsigned long n, double *x, double *y) {
	double sxx=0.0,sxy=0.0;
	unsigned long i;
	for (i=0;i<n;i++) {
		sxx+=x[i]*x[i];
		sxy+=x[i]*y[i];
	}
	return (float)(sxy/sxx);
}

float p80(unsigned long n, double *x) {
	unsigned long n80=0.8*n;
	double *z=(double *)malloc(n*sizeof(double));
	unsigned long i;
	for (i=0;i<n;i++) z[i]=x[i];
	qsort(z,n,sizeof(float),compare_double);
	float p80=z[n80];
	free(z);
	return p80;
}

int main(int argc, char **argv) {
	unsigned long n_nodes=2;
	float duplication_rate=0.3;
	char arc_distribution[100]="exponential";
	float arc_p1=1.0,arc_p2=1.0;
	char node_distribution[100]="lognormal";
	float node_p1=1.0,node_p2=10.0;
	char fileoutput[500]="maxplus_dupsplit.csv";
	char propagator[100]="maxsum";
	char mode[100]="scheduling";
	unsigned long n_nets=1;
	unsigned long n_avs=100;
	unsigned long n_nvs=100;
	int c;
	char *remain;
	for (c=1;c<argc;c+=2) {
		if (strcmp(argv[c],"-n_nets")==0) n_nets=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_avs")==0) n_avs=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_nvs")==0) n_nvs=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-arc_distribution")==0) strcpy(arc_distribution,argv[c+1]);
		if (strcmp(argv[c],"-arc_mu")==0) arc_p1=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-arc_sigma")==0) arc_p2=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-node_distribution")==0) strcpy(node_distribution,argv[c+1]);
		if (strcmp(argv[c],"-node_mu")==0) node_p1=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-node_sigma")==0) node_p2=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-n_nodes")==0) n_nodes=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-duplication_rate")==0) duplication_rate=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-propagator")==0) strcpy(propagator,argv[c+1]);
		if (strcmp(argv[c],"-mode")==0) strcpy(mode,argv[c+1]);
		if (strcmp(argv[c],"-fileoutput")==0) strcpy(fileoutput,argv[c+1]);
	}
	struct dinet_class dinet;
	long seed=-1000000;
	if (strcmp(arc_distribution,"zero")==0) n_avs=1;
	unsigned long sum=n_nets*n_avs*n_nvs;
	double *x=(double *)malloc(sum*sizeof(double));
	double *y=(double *)malloc(sum*sizeof(double));
	double *z=(double *)malloc(sum*sizeof(double));
	unsigned long sum_=n_nets*n_avs;
	double *y_=(double *)malloc(n_nets*n_avs*sizeof(double));
	double *z_=(double *)malloc(n_nets*n_avs*sizeof(double));
	double *y__=(double *)malloc(n_nvs*sizeof(double));
	double *z__=(double *)malloc(n_nvs*sizeof(double));
	unsigned long net,av,nv,i=0,i_=0,i__;
	for (net=0;net<n_nets;net++) {
		generators_duplication_split(&dinet,n_nodes,duplication_rate,(net+1)*seed);
		topology_ordering_topological(&dinet);
		for (av=0;av<n_avs;av++) {
			processes_arc_values(&dinet,arc_distribution,arc_p1,arc_p2,(av+1)*seed,mode);
			i__=0;
			for (nv=0;nv<n_nvs;nv++) {
				x[i]=processes_node_values(&dinet,node_distribution,node_p1,node_p2,(nv+1)*seed);
				processes_summax(&dinet,propagator);
				y[i]=processes_ouput_max_from_out_degree_0(&dinet);
				processes_summax(&dinet,"maxmax");
				z[i]=processes_ouput_max_from_out_degree_0(&dinet);
				y__[i__]=y[i];
				z__[i__]=z[i];
				i++;
				i__++;
			}
			y_[i_]=p80(n_nvs,y__);
			z_[i_]=p80(n_nvs,z__);
			i_++;
		}
		free_dinet(&dinet);
	}
	FILE *file=fopen(fileoutput,"wt");
	fprintf(file,"%s,%f,%f,%s,%f,%f,%lu,%f,%f,%f,%f,%f\n",arc_distribution,arc_p1,arc_p2,node_distribution,node_p1,node_p2,n_nodes,duplication_rate,slope(sum,x,y),variance(sum,x),slope(sum,z,y),slope(sum_,z_,y_));
	fclose(file);
	free(x);
	free(y);
	free(z);
	free(y_);
	free(z_);
	free(y__);
	free(z__);
	return 1;
}
