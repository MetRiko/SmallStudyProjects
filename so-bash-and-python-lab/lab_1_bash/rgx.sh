#!/bin/bash

url='https://en.wikipedia.org/wiki/Wikipedia:How_many_legs_does_a_horse_have%3F'
src=`wget -O - -q $url`

#echo "$src"

echo "$src" | grep -Eo '<a[^>]*>' | grep -Eo 'href="[^"]*" title="[^"]*">' |\
awk -F '"' '{print "> "$4,"-> \"",$2,"\""}'

#| while read -r line; do echo "$line"; done #| awk '{print $0}'

