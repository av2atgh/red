#python libs/real_clean.py

for i in {1..242}
do
	echo $i
	./real -net data/real_$i
done
