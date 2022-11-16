N_NETS=10
N_AVS=100
N_NVS=1000
ARC_DISTRIBUTION="zero"
ARC_MU=1
ARC_SIGMA=1
NODE_DISTRIBUTION="lognormal"
NODE_MU=1
NODE_SIGMA=1
#N_NODES=100
DUPLICATION_RATE=0.3
#MODE="testing"
MODE="scheduling"

HEAD="arc_distribution,arc_mu,arc_sigma,node_distribution,node_mu,node_sigma,n_nodes,duplication_rate,slope2input,input_variance,slope2maxmax,slope_p80,slope_blackswan\n"
FILEOUTPUT=".dupsplit.arc_distribution_"
ARC_SIGMA_LABEL=".arc_sigma_"
EXTENSION=".csv"
if [ "$MODE" = "scheduling" ]; then
	MODE_LABEL=".scheduling"
else
	MODE_LABEL=""
fi	

PROPAGATOR="maxsum"
#for ARC_DISTRIBUTION in zero exponential lognormal
for ARC_DISTRIBUTION in lognormal
do
#	printf "$HEAD" > maxplus.dupsplit.csv
#	for DUPLICATION_RATE in 0.1 0.4
	for DUPLICATION_RATE in 0.4
	do
#		for N_NODES in 1000 10000
		for N_NODES in 10000
		do
			printf "$DUPLICATION_RATE $N_NODES\n"
#			for NODE_SIGMA in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0
			for NODE_SIGMA in 1.5 1.6 1.7 1.8 1.9 2.0 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9 3.0 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8 3.9 4.0 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 5.0
#			for NODE_SIGMA in 0.1 0.3 0.5 0.7 0.9 1.1 1.3 1.5 1.7 1.9 2.1 2.3 2.5 2.7 2.9 3.1 3.3 3.5 3.7 3.9 4.1 4.3 4.5 4.7 4.9
			do
				./model -fileoutput "maxplus.csv" -propagator $PROPAGATOR -mode $MODE -arc_distribution "$ARC_DISTRIBUTION" -arc_mu $ARC_MU -arc_sigma $ARC_SIGMA -node_distribution "$NODE_DISTRIBUTION" -node_mu $NODE_MU -node_sigma $NODE_SIGMA -n_nodes $N_NODES -duplication_rate $DUPLICATION_RATE -n_nets $N_NETS -n_avs $N_AVS -n_nvs $N_NVS > log
				cat maxplus.csv >> maxplus.dupsplit.csv
				cat maxplus.csv
			done
		done
	done
	if [ "$ARC_DISTRIBUTION" = "zero" ]; then
		mv maxplus.dupsplit.csv "data/$PROPAGATOR$MODE_LABEL$FILEOUTPUT$ARC_DISTRIBUTION$EXTENSION"
	fi
	if [ "$ARC_DISTRIBUTION" = "exponential" ]; then
		mv maxplus.dupsplit.csv "data/$PROPAGATOR$MODE_LABEL$FILEOUTPUT$ARC_DISTRIBUTION$EXTENSION"
	fi
	if [ "$ARC_DISTRIBUTION" = "lognormal" ]; then
		mv maxplus.dupsplit.csv "data/$PROPAGATOR$MODE_LABEL$FILEOUTPUT$ARC_DISTRIBUTION$ARC_SIGMA_LABEL$ARC_SIGMA$EXTENSION"
	fi
done

rm maxplus.csv
