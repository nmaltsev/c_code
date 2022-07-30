#!/bin/zsh

for (( i=1;i<51; i++ ))
do
random_value=$(($RANDOM%51))
	if [[ $random_value -lt 10 ]]; then
	    echo -n 0$random_value' '
	else
	    echo -n $random_value' '
	fi
	
	if [[ $(($i%10)) -eq 0 ]]; then
	    echo
	fi
done

	
