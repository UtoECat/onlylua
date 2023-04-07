#!/bin/bash
# very useful to find where some symbol is defined...

echo "searching for $1 in source and header files..."

for fn in $(find . -name '*.lua' -type f)
do
	grep -H -n -i $1 $fn
done

