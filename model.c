#include "red.h"
#include "topology.h"
#include "generators.h"
#include "processes.h"

unsigned long n_networks=10;
unsigned long n_weights=10;
unsigned long n_inputs=1000;


int main(int argc, char **argv) {
	unsigned long n_nodes=100;
	float duplication_rate=0.3;
	char *load_distribution="lognormal";
	float mu=1.0,sigma=10.0;
	char c;
	for (c=0;c<argc;c+=2) {
		if strcmp(argv[c],"-n_nodes") n_nodes=strtol(argv[c+1],&remain,0);
		if strcmp(argv[c],"-duplication_rate") duplication_rate=strtod(argv[c+1],&remain,0);
		if strcmp(argv[c],"-distribution") strcpy(load_distribution,argv[c+1]);
		if strcmp(argv[c],"-mu") p1=strtod(argv[c+1],&remain,0);
		if strcmp(argv[c],"-sigma") p2=strtod(argv[c+1],&remain,0);
	}
	float p1=mu,p2=sigma;
	float x,y;
	struct dinet_class dinet;
	long seed=-1000000;
	generators_duplication_split(&dinet,n_nodes,duplication_rate,seed);
	topology_ordering_topological(&dinet);
	x=processes_node_values(&dinet,load_distribution,p1,p2,seed);
	processes_arc_values(&dinet,"zero",p1,p2,seed);
	processes_summax(&dinet,"maxsum");
	y=processes_ouput_max_from_out_degree_0(&dinet);
	printf("%f\t%f\t%f\n",x,y,y/x);
	return 1;
}
