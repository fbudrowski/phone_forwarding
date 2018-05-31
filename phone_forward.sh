#!/bin/bash

#echo "Skrypt: "
#echo $1
#echo $2
#echo $3

if [ $# -ne 3 ]; then
	echo "Użycie: ./phone_forward.sh <program> <operacje> <liczba>"
fi;

"$1" "-verskr" <$2 1>temp.out 2>temp.err
echo "" > blank.err
#cat temp.err
#cat blank.err

if [[ "$(diff -qBZ temp.err blank.err)" != "" ]]; then
	echo "Złe dane wejściowe"
fi

rm temp.err blank.err


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
