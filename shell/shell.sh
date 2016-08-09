#!/bin/bash

i=0
str=''
arr=('|' '/' '-' '\\')
index=0
while [ $i -le 100 ]
do 
    printf "[%-101s][%d%%][%c]\r" "$str" "$i" "${arr[$index]}"
    str=${str}'#'
    sleep 0.1
    let i++
    let index=i%4
done




