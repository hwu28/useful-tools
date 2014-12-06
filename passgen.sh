#!/bin/bash

#arguements are -d for delete, and -c for password length

delete="\n\t"
length=5
length_set=false

function usage(){
	echo
	echo "Usage:" >&2
	echo "passgen [-d regex] [-c length]" >&2
	echo >&2
	echo "-d string" >&2
	echo "   Prevents characters in \"regex\", as well as the default \\t and \\n from being used" >&2
	echo >&2
	echo "-c length" >&2
	echo "   Tell passgen to use a pasword length of \"length\", instead of the default 5 (must be a natural number)" >&2
	echo
	exit 1
}

while [ $# -ne 0 ]; do
	case "$1" in
	"-d")
		shift
		if [ $# -eq 0 ]; then
			usage
		fi
		delete="$delete$1"
		;;
	"-c")
		shift
		expr "$1 + 1" &> /dev/null
		if [ $? -ne 0 ]; then
			usage
		elif [ $length_set ] | [ $1 -lt 1 ]; then
			usage
		fi
		length_set=true
		length=$1
		;;
	*)
		usage
		;;
	esac
	shift
done

strings -n 1 /dev/urandom | tr -d "$delete" | head -c "$length"
echo
exit 0
