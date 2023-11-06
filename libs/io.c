#include "io.h"

void write_edgelist(struct dinet_class *dinet, const char *fileoutput) {
	printf("xxx %s\n",fileoutput);
	FILE *file=fopen(fileoutput,"wt");
	fprintf(file,"pred,succ\n");
	unsigned long i;
	for (i=0;i<(*dinet).n_arcs;i++) {
		printf("%lu,%lu\n",(*dinet).arc[i].pred,(*dinet).arc[i].succ);
		fprintf(file,"%lu,%lu\n",(*dinet).arc[i].pred,(*dinet).arc[i].succ);
	}
	fclose(file);
	return;
}

void read_task_real(struct dinet_class *dinet, const char *fileinput) {
	printf("read_task_real start...\n");
	char *aline,*token,*remain,line[LINEMAX];
	FILE *file=fopen(fileinput,"rt");
	unsigned long i,j,r,id,n_arcs=0,n_nodes=0;
	//
	// get relations
	//
	printf("reading %s\n",fileinput);
	fgets(line,LINEMAX,file);
	while (fgets(line,LINEMAX,file)!=NULL) n_arcs++;
	printf("%lu arcs\n",n_arcs);
	(*dinet).n_arcs=n_arcs;
	(*dinet).arc=(struct arc_class *)malloc(n_arcs*sizeof(struct arc_class));
	unsigned long *task_id=(unsigned long *)malloc(n_arcs*sizeof(long));
	rewind(file);
	fgets(line,LINEMAX,file);
	for (r=0;r<n_arcs;r++) {
		fgets(line,LINEMAX,file);
		aline=strdup(line);
		// pred
		token=strsep(&aline,",\n");
		id=strtol(token,&remain,0);
		i=0;
		while (i<n_nodes && id!=task_id[i]) i++;
		if (i==n_nodes) {
			task_id[i]=id;
			n_nodes++;
		}
		(*dinet).arc[r].pred=i;
		// pred
		token=strsep(&aline,",\n");
		id=strtol(token,&remain,0);
		i=0;
		while (i<n_nodes && id!=task_id[i]) i++;
		if (i==n_nodes) {
			task_id[i]=id;
			n_nodes++;
		}
		(*dinet).arc[r].succ=i;
	}
	fclose(file);
//	free(task_id);
	printf("%lu nodes...\n",n_nodes);
	//
	// get nodes
	//
	(*dinet).n_nodes=n_nodes;
	(*dinet).node=(struct dinode_class *)malloc(n_nodes*sizeof(struct dinode_class));
	for (i=0;i<n_nodes;i++) (*dinet).node[i].ou_degree=(*dinet).node[i].in_degree=0;
	for (r=0;r<n_arcs;r++) { 
		i=(*dinet).arc[r].pred;
		j=(*dinet).arc[r].succ;
		(*dinet).node[i].ou_degree++;
		(*dinet).node[j].in_degree++;
	}
	for (i=0;i<n_nodes;i++) {
		(*dinet).node[i].ou_arc=(unsigned long *)malloc((*dinet).node[i].ou_degree*sizeof(long));
		(*dinet).node[i].in_arc=(unsigned long *)malloc((*dinet).node[i].in_degree*sizeof(long));
		(*dinet).node[i].ou_degree=(*dinet).node[i].in_degree=0;
	}
	for (r=0;r<n_arcs;r++) { 
		i=(*dinet).arc[r].pred;
		j=(*dinet).arc[r].succ;
		(*dinet).node[i].ou_arc[(*dinet).node[i].ou_degree]=r;
		(*dinet).node[j].in_arc[(*dinet).node[j].in_degree]=r;
		(*dinet).node[i].ou_degree++;
		(*dinet).node[j].in_degree++;
	}
	printf("%lu nodes and %lu arcs\n",n_nodes,n_arcs);
	printf("read_task_real done.\n");
	return;
}
