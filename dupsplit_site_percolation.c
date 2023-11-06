#include "red.h"
#include "topology.h"
#include "generators.h"
#include <float.h>
#include <string.h>

int main(int argc, char **argv) {
	//
	// input
	//
	unsigned long n_nets=1;
	unsigned long n_percolations=1;
	unsigned long n_nodes=10000;
	float duplication_rate=0.3;
	int c;
	char *remain;
	for (c=1;c<argc;c+=2) {
		if (strcmp(argv[c],"-n_nodes")==0) n_nodes=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_nets")==0) n_nets=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_percolations")==0) n_percolations=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-duplication_rate")==0) duplication_rate=strtod(argv[c+1],&remain);
	}
	char fileoutput[500];
	sprintf(fileoutput,"data/dupsplit_site_percolation_duplication_rate_%.2f_nodes_%lu_nets_%lu_percolations_%lu.csv",duplication_rate,n_nodes,n_nets,n_percolations);
	//
	// calculations
	//
	struct dinet_class dinet;
	long seed=-1000000;
	long percolation_seed=-1000000;
	unsigned long net,i,per;
	float p_site;
	unsigned long *components_size=(unsigned long *)malloc(n_nodes*sizeof(long));
	#define NBINS 100
	unsigned long b,n_bins=NBINS;
	float smean,smax,mean_component_size[NBINS], mean_max_component_size[NBINS];
	for (b=0;b<n_bins;b++) mean_component_size[b]=mean_max_component_size[b]=0;
	for (net=0;net<n_nets;net++) {
		generators_duplication_split(&dinet,n_nodes,duplication_rate,(net+1)*seed);
		topology_ordering_topological(&dinet);
		for (b=0;b<n_bins;b++) {
			p_site = 0.01 + 0.99*(float)b/(n_bins-1);
			for (per=0;per<n_percolations;per++) {
				components(&dinet,p_site,(per+1)*percolation_seed,components_size);
				smax=0.0;
				smean=0.0;
				i=0;
				while (i<n_nodes && components_size[i]>0) {
					smean+=components_size[i];
					if (components_size[i]>smax) smax=components_size[i];
					i++;
				}
				smean-=smax;
				mean_component_size[b]+=smean/i;
				mean_max_component_size[b]+=smax/n_nodes;
			}
		}
		free_dinet(&dinet);
	}
	FILE *file=fopen(fileoutput,"wt");
	fprintf(file,"p_site,mean_component_size,mean_max_component_size\n");
	for (b=0;b<n_bins;b++) {
		p_site = 0.01 + (float)b/(n_bins-1);
		mean_component_size[b]/=n_nets*n_percolations;
		mean_max_component_size[b]/=n_nets*n_percolations;
		fprintf(file,"%f,%f,%f\n",p_site,mean_component_size[b],mean_max_component_size[b]);
	}
	fclose(file);
	return 1;
}
