#
#   duplication-split model simulations
#

NNETS=1
NNODES=1000000
for Q in 0.1 0.2 0.3 0.4 0.5 0.6
do
	./dupsplit_generation -n_nodes $NNODES -duplication_rate $Q -n_nets $NNETS -duration_dist "constant"
	./dupsplit_generation -n_nodes $NNODES -duplication_rate $Q -n_nets $NNETS
done

#
#   code to generate figures
#

python dupsplit_duration_figs.py
