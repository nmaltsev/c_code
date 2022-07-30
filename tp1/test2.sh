#!/bin/zsh
c=$1
l=$2

for (( i=1; i<=$c; i++ ))
do
    for (( e=1;e<=$l;e++ ))
    do
	max_in_column=$(($e*$c))
	num=$(( $e*$i ))
	num=$(printf "%*d" $#max_in_column $num)
	echo -n "$num "
    done
    echo 
done

	
