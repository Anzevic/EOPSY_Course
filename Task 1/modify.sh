#!/bin/sh

[ -z "$1" ] && exit -1;

if [ $1 = "--help" ] || [ $1 = "-h" ]; then
	echo 'Usage:          modify [-r] [-l|-u] <dir/file names...>
						  modify [-h]
			Options:
				-r, --recursive: recursive execution
				-l, --lowercase: lowercase filename
				-u, --uppercase: uppercase filename
				-h, --help: help options'
exit 0
fi


if [ $1 = "--recursive" ] || [ $1 = "-r" ] ; then
	echo "Recursive execution"
	if [ $2 = "--lowercase" ] || [ $2 = "-l" ]; then
		
		for i in $( echo $3 | grep [A-Z] );
			do mv -i $i `echo $i | tr 'A-Z' 'a-z'`;
		done
		if [ "$#" -gt 3 ]; then
			shift
			shift
			shift
			./modify "-r" "-l" "$@"
		fi
	elif [ $2 = "--uppercase" ] || [ $2 = "-u" ]; then
		for i in $( echo $3 | grep [a-z] );
			do mv -i $i `echo $i | tr 'a-z' 'A-Z'`;
		done
		if [ "$#" -gt 3 ]; then
			shift
			shift
			shift
			./modify "-r" "-u" "$@"
		fi
	fi

else
	echo "Non-recursive execution"
	if [ $2 = "--lowercase" ] || [ $2 = "-l" ]; then
		shift
		while [ "$#" -ne 0 ]; do
			for i in $( echo ${1%.} | grep [A-Z] );
				do mv -i $i `echo $i | tr 'A-Z' 'a-z'`;
			done
			shift
		done
	elif [ $2 = "--uppercase" ] || [ $2 = "-u" ]; then
		shift
		while [ "$#" -ne 0 ]; do
			for i in $( echo ${1%.} | grep [a-z] );
				do mv -i $i `echo $i | tr 'a-z' 'A-Z'`;
			done
			shift
		done
	fi
fi