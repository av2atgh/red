#ifndef _TOPOLOGY_H_
#define _TOPOLOGY_H_

#include "red.h"

// // flags

// // classes

// // functions

extern void topology_ordering_topological(struct dinet_class *dinet);
extern void topology_generation(struct dinet_class *dinet);
extern unsigned long average_number_of_nodes_by_distance(struct dinet_class *dinet, float *nodes_at_distance_array);
extern unsigned long one_step_renormalization(struct dinet_class *dinet, unsigned long box_size, struct dinet_class *dinet_b);
extern void q_max_likelihod_schedule(struct dinet_class *dinet);

#endif
