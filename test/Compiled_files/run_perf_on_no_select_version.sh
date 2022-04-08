#!/bin/bash

clang -g -O3 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -mllvm -phi-node-folding-threshold=0 -mllvm -two-entry-phi-node-folding-threshold=0 -mllvm -simplifycfg-hoist-common=false -mllvm -simplifycfg-sink-common=0 -mllvm -simplifycfg-hoist-cond-stores=false -mllvm -simplifycfg-merge-cond-stores=false -mllvm -simplifycfg-branch-fold-threshold=0 -mllvm -simplifycfg-branch-fold-common-dest-vector-multiplier=0  -S -emit-llvm dijkstra.c -o dijkstra_no_select.ll


for value in {1..100}; do

  rm -f *.out
  rm -f *.gz

  clang dijkstra_no_select.ll -o dijkstra_no_select.out
  perf stat -o results/dijkstra_no_select_cache_results_"$value".txt -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses gzip dijkstra_no_select.out

  rm -f *.out
  rm -f *.gz

  clang dijkstra_no_select.ll -o dijkstra_no_select.out
  perf stat -o results/dijkstra_no_select_branch_results_"$value".txt -e branch-instructions,branch-loads,branch-load-misses gzip dijkstra_no_select.out

  rm -f *.out
  rm -f *.gz

  clang dijkstra_no_select.ll -o dijkstra_no_select.out
  perf stat -o results/dijkstra_no_select_time_Result_"$value".txt -e duration_time gzip dijkstra_no_select.out

  echo "Round $value"
done

rm -f *.out
rm -f *.gz
