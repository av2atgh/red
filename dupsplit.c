#include "red.h"
#include "rand.h"
#include "io.h"
#include "topology.h"
#include "generators.h"
#include <float.h>
#include <string.h>

int main(int argc, char **argv) {
	unsigned long n_nets=1;
	unsigned long n_nodes=1000000;
	float duplication_rate=0.3;
	char fileoutput[500]="maxplus_dupsplit.csv";
	int c;
	char *remain;
	for (c=1;c<argc;c+=2) {
		if (strcmp(argv[c],"-n_nets")==0) n_nets=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-n_nodes")==0) n_nodes=strtol(argv[c+1],&remain,0);
		if (strcmp(argv[c],"-duplication_rate")==0) duplication_rate=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-n_nets")==0) n_nets=strtod(argv[c+1],&remain);
		if (strcmp(argv[c],"-fileoutput")==0) strcpy(fileoutput,argv[c+1]);
	}
	struct dinet_class dinet;
	long seed=-1000000;
	unsigned long net,i,box_size,n_boxes,diameter,diameter_max=0;
        float *nodes_at_distance_array=(float *)malloc(n_nodes*sizeof(float));
        float *ave_nodes_at_distance_array=(float *)malloc(n_nodes*sizeof(float));
        float *ave_boxes_at_distance_array=(float *)malloc(n_nodes*sizeof(float));
        for (i=0;i<n_nodes;i++) {
		ave_nodes_at_distance_array[i]=0;
		ave_boxes_at_distance_array[i]=0;
	}
	ave_boxes_at_distance_array[0]=n_nodes*n_nets;
	unsigned long d,sum,dmin=1,dmax=100;
	float x,y,xx,xy,mass,slope,slope_ave_1=0.0,slope_ave_2=0.0;
	char fileoutput_edgelist[500];
	for (net=0;net<n_nets;net++) {
		generators_duplication_split(&dinet,n_nodes,duplication_rate,(net+1)*seed);
//		if (net==0) {
//			sprintf(fileoutput_edgelist,"data/dupsplit_q%.2f_n%lu_edge_list.csv",duplication_rate,n_nodes);
//			write_edgelist(&dinet,fileoutput_edgelist);
//		}
		topology_ordering_topological(&dinet);
		diameter=average_number_of_nodes_by_distance(&dinet, nodes_at_distance_array);
		if (diameter>diameter_max) diameter_max=diameter;
		//
		// calculate fractal dimension
		//
		mass=0;
		for (d=0;d<dmin;d++) mass+=nodes_at_distance_array[d]/n_nodes;
		sum=0;
		x=y=xx=xy=0.0;
		for (d=dmin;d<=dmax;d++) {
			// we use d+1 bacause mass(d) = number of nodes at d'<d
			mass+=nodes_at_distance_array[d];
			x+=log(d+1);
			y+=log(mass);
			xy+=log(mass)*log(d+1);
			xx+=log(d+1)*log(d+1);
			sum++;
		}
		x/=sum;
		y/=sum;
		xx/=sum;
		xy/=sum;
		slope = (xy-x*y)/(xx-x*x);
		slope_ave_1+=slope;
		slope_ave_2+=slope*slope;
		//
		for (i=0;i<=diameter;i++) ave_nodes_at_distance_array[i]+=nodes_at_distance_array[i];
/*
		for (box_size=1;box_size<diameter;box_size++) {
			n_boxes=one_step_renormalization(&dinet, box_size, NULL);
			ave_boxes_at_distance_array[box_size]+=n_boxes;
		}
		ave_boxes_at_distance_array[diameter]+=1;
*/
		free_dinet(&dinet);
	}
	//
	// average distance
	//
	float dmean=0,dsum=0;
	for (i=0;i<=diameter_max;i++) {
		dsum+=ave_nodes_at_distance_array[i];
		dmean+=i*ave_nodes_at_distance_array[i];
	}
        dmean/=dsum;
	//
	// calculate cumulative node count at distance
	// normalize box count
	//
	float mean,sigma;
	if (n_nets==1) {
		mean=slope_ave_1;
		sigma=0;
	}
	else {
		mean=slope_ave_1/n_nets;
		sigma=slope_ave_2/n_nets-mean*mean;
		sigma=(sigma>0)?sqrt(sigma):0;
	}
	ave_nodes_at_distance_array[0]/=n_nets;
	for (i=1;i<=diameter_max;i++) {
		ave_nodes_at_distance_array[i]=ave_nodes_at_distance_array[i-1] + ave_nodes_at_distance_array[i]/n_nets;
		ave_boxes_at_distance_array[i]/=n_nets;
	}
	//
	// output
	//
	char filename[500];
	sprintf(filename,"%s_df.csv",fileoutput);
	FILE *file=fopen(filename,"wt");
	fprintf(file,"mean,sigma,dmean\n%f,%f,%f\n",mean,sigma,dmean);
	sprintf(filename,"%s_fractal.csv",fileoutput);
	file=fopen(filename,"wt");
        fprintf(file,"d,m,b\n");
        for (i=0;i<=diameter_max;i++) fprintf(file,"%lu,%f,%f\n",i+1,ave_nodes_at_distance_array[i],ave_boxes_at_distance_array[i]);
	fclose(file);
	free(nodes_at_distance_array);
	free(ave_nodes_at_distance_array);
	free(ave_boxes_at_distance_array);
	return 1;
}
