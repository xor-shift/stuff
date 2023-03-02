#!/bin/bash

aeiou=("scc.sh" "stuff/CMakeLists.txt" "CMakeLists.txt" "main.cpp")

for f in ./stuff/libs/*; do
  aeiou+=("$f/include" "$f/test" "$f/CMakeLists.txt")
done

scc ${aeiou[*]}
