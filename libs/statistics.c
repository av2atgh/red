#include "stats.h"


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
float statistics_power_law_exponent_max_likelihod(unsigned long n, unsigned long *x, unsigned long xmax) {
	// Based on "Routine for Golden Section Search", Numerical Recipes in C
	float f0,f1,f2,x0,x1,x2,x3;
	float ax=1.0;
	float bx=2.0;
	float cx=3.0;
	f0=likelihood(n,x,xmax,ax);
	f1=likelihood(n,x,xmax,bx);
	f2=likelihood(n,x,xmax,cx);
	//printf("\t%f,%f,%f\n",f0,f1,f2);
	while (f2<f1) {
		bx++;
		cx++;
		f1=likelihood(n,x,xmax,bx);
		f2=likelihood(n,x,xmax,cx);
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
	f1=likelihood(n,x,xmax,x1);
	f2=likelihood(n,x,xmax,x2);
	while ( fabs(x3-x0) > TOLERANCE*(fabs(x1)+fabs(x2)) ) {
		if (f2 < f1) {
			x0=x1;
			x1=x2;
			x2=R*x1+(1.0-R)*x3;
			f1=f2;
			f2=likelihood(n,x,xmax,x2);
		}
		else {
			x3=x2;
			x2=x1;
			x1=R*x2+(1.0-R)*x0;
			f2=f1;
			f1=likelihood(n,x,xmax,x1);
		}
	}
	if (f1 < f2) return 1.0/x1;
	else return 1.0/x2;
}
#undef R
#undef TOLERANCE


void statistics_histogram_log_binning(const char *filename, const char *extension, unsigned long first, unsigned long last, unsigned long *x) {
	printf("function,histogram_log_binning,start\n");
	unsigned long i,max=x[first];
	for (i=first+1;i<=last;i++) if (x[i]>max) max=x[i];
	unsigned long b,base=2;
	unsigned long n_bins=(unsigned long)(log(max)/log(base))+1;
	max=pow(base,n_bins);
	printf("report,max,%lu\n",max);
	unsigned long *bin_size,*count;
	bin_size=(unsigned long *)malloc(n_bins*sizeof(long));
	count=(unsigned long *)malloc(n_bins*sizeof(long));
	for (b=0;b<n_bins;b++) bin_size[b]=count[b]=0;
	for (i=1;i<max;i++) {
		b=log(i)/log(base);
		bin_size[b]++;
	}
	for (i=first;i<=last;i++) if (x[i]>0) {
		b=(unsigned long)(log(x[i])/log(base));
		count[b]++;
	}
	FILE *file;
	char filename_tmp[1000];
	sprintf(filename_tmp,"%s%s.dis.log_binning.csv",filename,extension);
	file=fopen(filename_tmp,"wt");
	fprintf(file,"bin,prob\n");
	for (b=0;b<n_bins;b++) if (count[b]>0) fprintf(file,"%f,%e\n",pow(base,b),(float)count[b]/(bin_size[b]*(last-first+1)));
	fclose(file);
	// free memory
	free(bin_size);
	free(count);
	printf("function,histogram_log_binning,end\n");
	return;
}
