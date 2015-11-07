#!/bin/bash

echo "------Testing AND------"
ls && ps
a && ps
sleep 5
ls && b
ls && b && a
ls && b && ps
a && b && ps
a && b && c
a && ls && b

echo "_________End And______"
echo "-------Testing Semi-Colon-----"
ls;ps
a;ps
ls;b
ls;ps;a
a;b;c
echo"____________End Semi-Colon____"

echo
echo "---------Testing OR -------"
ls || a
a || ls
a || b
a | b || ls
echo "_________End OR __________"

echo
echo "---------Testint MIXED Commands___"
ls || ps && echo hello
a || ps && echo hello
ls || b && echo hello
ls && ps || echo hello
a && ps || ps echo hello
ls && b || echo hello
ls; ps && ls; a || b; ls || ps && echo hello
a;b;ls;a || ps && ls;
echo "__________End Mixed Conmmands___"


