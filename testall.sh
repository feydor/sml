#!/bin/bash
for file in examples/*.smol
do
  ./build/smol "$file" >> res.out
  echo "" >> res.out
done

