#!/bin/bash
rm -f -- res.out
for file in examples/*.smol
do
  ./build/smol "$file" >> res.out
  echo "" >> res.out
done

