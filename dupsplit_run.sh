#N=100000
N=100
NETS=1
#for q in {0..55}
for q in 10 30 50
do
	if [ $q -lt 10 ]
	then
		FILEOUTPUT="data/dupsplit_q0.0${q}_n${N}"
		echo $FILEOUTPUT
		./dupsplit -n_nodes $N -duplication_rate 0.0$q -n_nets $NETS -fileoutput $FILEOUTPUT
	else
		FILEOUTPUT="data/dupsplit_q0.${q}_n${N}"
		echo $FILEOUTPUT
		./dupsplit -n_nodes $N -duplication_rate 0.$q -n_nets $NETS -fileoutput $FILEOUTPUT
	fi
done
