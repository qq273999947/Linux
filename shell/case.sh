#!/bin/bash 

read val
case $val in
    a )
        echo "enter  val is a"
    ;;
    b )
        echo "enter val is b"
    ;;
    c )
        echo "wow ,this is c"
    ;;
    * )
        echo "I dont know"
    ;;
esac
