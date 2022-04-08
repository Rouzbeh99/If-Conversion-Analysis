#!/bin/bash
clang -g -O3 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -S -emit-llvm dijkstra.c -o dijkstra_full_opt.ll

for value in {1..100}; do

  rm -f *.out
  rm -f *.gz

  clang dijkstra_full_opt.ll -o dijkstra_full_opt.out
  perf stat -o results/dijkstra_full_opt_cache_Result_"$value".txt -e L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses gzip dijkstra_full_opt.out

  rm -f *.out
  rm -f *.gz

  clang dijkstra_full_opt.ll -o dijkstra_full_opt.out
  perf stat -o results/dijkstra_full_opt_branch_Result_"$value".txt -e branch-instructions,branch-loads,branch-load-misses gzip dijkstra_full_opt.out

  rm -f *.out
  rm -f *.gz

  clang dijkstra_full_opt.ll -o dijkstra_full_opt.out
  perf stat -o results/dijkstra_full_opt_time_Result_"$value".txt -e duration_time gzip dijkstra_full_opt.out


  echo "Round $value"
done

rm -f *.out
rm -f *.gz
