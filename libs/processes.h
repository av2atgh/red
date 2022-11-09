#ifndef _PROCESSES_H_
#define _PROCESSES_H_

#include "red.h"
#include "rand.h"

// // flags

// // classes

// // functions

extern float processes_node_values(struct dinet_class *dinet, char *load_distribution, float p1, float p2, long seed0);
extern void processes_arc_values(struct dinet_class *dinet, char *load_distribution, float p1, float p2, long seed0, const char *mode);
extern void processes_summax(struct dinet_class *dinet, char *use_rule);
extern double project_dvalue(struct dinet_class *dinet);

#endif
