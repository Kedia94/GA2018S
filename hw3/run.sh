#1/bin/sh
for i in 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20
do
	echo "running $i"
	make run >> result.txt
done
