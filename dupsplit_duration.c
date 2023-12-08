#include "red.h"
#include "topology.h"
#include "generators.h"
#include "processes.h"
#include <float.h>
#include <string.h>

int main(int argc, char **argv) {
	unsigned long n_nets=1;
	unsigned long n_nodes=10000;
	float duplication_rate=0.3;
	int c;
	char *remain;
	float dmean=30.0;
	for (c=1;c<argc;c+=2) {
		if (strcmp(argv[c],"-n_nets")==0) n_nets=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_nodes")==0) n_nodes=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-duplication_rate")==0) duplication_rate=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-dmean")==0) dmean=strtod(argv[c+1],&remain);
	}
	float dmin=1.0/(dmean*n_nodes);
	struct dinet_class dinet;
	long seed=-1000000;
	unsigned long net,i,g;
	unsigned long *count=(unsigned long *)malloc(n_nodes*sizeof(long));
	for (g=0;g<n_nodes;g++) count[g]=0;
	//
	// simulations
	//
	char filename[500];
	FILE *file;
	double entropy_ave=0.0,width_ave=0.0,length_ave=0.0,width;
        char head[500];
	sprintf(head,"data/dupsplit_q%.2f_n%lu_dmean%.0f",duplication_rate,n_nodes,dmean);
        printf("%s\n",head);
	for (net=0;net<n_nets;net++) {
		generators_duplication_split(&dinet,n_nodes,duplication_rate,(net+1)*seed,dmin);
		topology_ordering_topological(&dinet);
		topology_generation(&dinet);
		for (i=0;i<n_nodes;i++) count[dinet.node[i].generation]++;
		g=0;
		width=0.0;
		while (g<n_nodes && count[g]>0) {
			width+=count[g];
			entropy_ave+=lgamma(count[g]+1);
			count[g]=0;
			g++;
		}
		if (g>0) width/=g;
		width_ave+=width;
		length_ave+=g;
		//
		//      scheduling with intrinsic durations
		//
		processes_arc_values(&dinet,"given",0,0,seed,"scheduling");
		sprintf(filename,"%s_i%lu_nodes.csv",head,net);
		file=fopen(filename,"wt");
                fprintf(file,"in_degree,ou_degree,generation,duration,total_float\n");
		for (i=0;i<dinet.n_nodes;i++) {
			fprintf(file,"%lu,%lu,%lu,%e,%e\n",dinet.node[i].in_degree,dinet.node[i].ou_degree,dinet.node[i].generation,dinet.node[i].duration,dinet.node[i].total_float);
		}
		fclose(file);
		free_dinet(&dinet);
	}
	width_ave/=n_nets;
	length_ave/=n_nets;
	entropy_ave/=n_nets;
	//
	// output
	//
	sprintf(filename,"data/dupsplit_generation_duplication_rate%.2f_n_nodes%lu_n_nets%lu_df.csv",duplication_rate,n_nodes,n_nets);
	file=fopen(filename,"wt");
	fprintf(file,"width,length,entropy\n%f,%f,%f\n",width_ave,length_ave,entropy_ave);
	fclose(file);
	free(count);
	return 1;
}
