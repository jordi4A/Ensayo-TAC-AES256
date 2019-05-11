#!/bin/bash

# A partir del 16 se queda colgado no se por qué
for i in {1..15}; do
    ./test.elf $i
done
