#include "red.h"
#include "io.h"
#include "topology.h"
#include <float.h>
#include <string.h>

int main(int argc, char **argv) {
	char net[500]="";
	int c;
	for (c=1;c<argc;c+=2) {
		if (strcmp(argv[c],"-net")==0) strcpy(net,argv[c+1]);
	}
	if (strcmp(net,"")==0) {
		printf("real -net NET_PATH_AND_NAME_PREFIX (removing _edge_list.csv)\n");
		exit(1);
	}
	printf("inputs in\n");
	//
	// input activity network
	//
	struct dinet_class dinet;
	char filename[500];
	sprintf(filename,"%s_edge_list.csv",net);
	read_task_real(&dinet,filename);
	q_max_likelihod_schedule(&dinet);
	topology_ordering_topological(&dinet);
	//
	// calculate distance metrics
	//
	unsigned long n_nodes=dinet.n_nodes;       
	unsigned long i,box_size,n_boxes,diameter;
	float *nodes_at_distance_array=(float *)malloc(n_nodes*sizeof(float));
	float *boxes_at_distance_array=(float *)malloc(n_nodes*sizeof(float));
	for (i=0;i<n_nodes;i++) boxes_at_distance_array[i]=0;
	diameter=average_number_of_nodes_by_distance(&dinet, nodes_at_distance_array);
	for (box_size=1;box_size<diameter;box_size++) {
		n_boxes=one_step_renormalization(&dinet, box_size, NULL);
		boxes_at_distance_array[box_size]+=n_boxes;
	}
	for (i=1;i<=diameter;i++) nodes_at_distance_array[i]=nodes_at_distance_array[i-1] + nodes_at_distance_array[i];
	//
	// output
	//
	FILE *file;
	sprintf(filename,"%s_q.csv",net);
	file=fopen(filename,"wt");
	fprintf(file,"q_ou,q_in\n%f,%f\n",dinet.q_ou,dinet.q_in);
	fclose(file);
	sprintf(filename,"%s_fractal.csv",net);
	file=fopen(filename,"wt");
        fprintf(file,"d,m,b\n");
        for (i=0;i<=diameter;i++) fprintf(file,"%lu,%f,%f\n",i+1,nodes_at_distance_array[i],boxes_at_distance_array[i]);
	fclose(file);
	free(nodes_at_distance_array);
	free(boxes_at_distance_array);
	free_dinet(&dinet);
	return 1;
}
