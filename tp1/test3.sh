#!/bin/zsh
nb=$1
value=1
c=''
for (( i=1; i<=$nb ; i++ ))
do
    value=$(($value*$i))
    if [[ $i -eq 1 ]]; then
	sign="="
    else
	sign="*"
    fi
    c="$i$sign$c"
done

echo "$nb!=$c$value"
