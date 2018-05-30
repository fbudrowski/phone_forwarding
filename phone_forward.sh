#!/bin/bash

if [ $# -ne 3 ]; then
	echo "Użycie: ./phone_forward.sh <program> <operacje> <y>"
fi;
echo "NEW xd" > temp.in
cat $2 >> temp.in
echo "? $3" >> temp.in 
$1 < temp.in > temp.out
#cat $2
#echo "? $3"

echo "NEW xd" > temp.in
cat $2 >> temp.in
for i in `cat temp.out`; do
	echo "$i ?" >> temp.in
done;

$1 < temp.in > temp.out2

: 'echo "IN"
cat temp.in
echo "OUT"
cat temp.out
echo "OUT2"
cat temp.out2
'

IFS=$'\n'
read -d '' -r -a lines1 < temp.out
read -d '' -r -a lines2 < temp.out2

#echo $lines1
#echo $lines2

LEN=${#lines1[@]}

#echo $LEN
for (( i=0; i<$LEN; i++)); do
	#echo $i
	#echo ${lines1[i]}
	#echo ${lines2[i]}
	if [ ${lines2[i]} -eq $3 ]; then
		echo ${lines1[i]}
	fi
done;


rm temp.in temp.out temp.out2
