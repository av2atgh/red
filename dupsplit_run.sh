N=1000000
NETS=1
#for q in {0..55}
#for q in 10 20 30 40 50 60 70 80 90
for q in 80 90
do
	echo $q
	if [ $q -lt 10 ]
	then
		FILEOUTPUT="data/dupsplit_q0.0${q}_n${N}_nnets${NETS}"
		echo $FILEOUTPUT
		./dupsplit -n_nodes $N -duplication_rate 0.0$q -n_nets $NETS -fileoutput $FILEOUTPUT
	else
		FILEOUTPUT="data/dupsplit_q0.${q}_n${N}_nnets${NETS}"
		echo $FILEOUTPUT
		./dupsplit -n_nodes $N -duplication_rate 0.$q -n_nets $NETS -fileoutput $FILEOUTPUT
	fi
done
