#!/bin/sh

echo Motion History 1s Buffer 2
./motion -d 1 $1 >motion-1-d1.log
./motion -d 1 $2 >motion-2-d1.log
echo Motion History 2s Buffer 2
./motion -d 2 $1 >motion-1-d2.log
./motion -d 2 $2 >motion-2-d2.log
echo Motion History 3s Buffer 2
./motion -d 3 $1 >motion-1-d3.log
./motion -d 3 $2 >motion-2-d3.log
echo Motion History 5s Buffer 2
./motion -d 5 $1 >motion-1-d5.log
./motion -d 5 $2 >motion-2-d5.log
echo Motion History 10s Buffer 2
./motion -d 10 $1 >motion-1-d10.log
./motion -d 10 $2 >motion-2-d10.log

echo Motion History 1s Buffer 4
./motion -d 1 -s 4 $1 >motion-1-d1s4.log
./motion -d 1 -s 4 $2 >motion-2-d1s4.log
echo Motion History 2s Buffer 4
./motion -d 2 -s 4 $1 >motion-1-d2s4.log
./motion -d 2 -s 4 $2 >motion-2-d2s4.log
echo Motion History 3s Buffer 4
./motion -d 3 -s 4 $1 >motion-1-d3s4.log
./motion -d 3 -s 4 $2 >motion-2-d3s4.log
echo Motion History 5s Buffer 4
./motion -d 5 -s 4 $1 >motion-1-d5s4.log
./motion -d 5 -s 4 $2 >motion-2-d5s4.log
echo Motion History 10s Buffer 4
./motion -d 10 -s 4 $1 >motion-1-d10s4.log
./motion -d 10 -s 4 $2 >motion-2-d10s4.log

echo Motion History 1s Buffer 8
./motion -d 1 -s 8 $1 >motion-1-d1s8.log
./motion -d 1 -s 8 $2 >motion-2-d1s8.log
echo Motion History 2s Buffer 8
./motion -d 2 -s 8 $1 >motion-1-d2s8.log
./motion -d 2 -s 8 $2 >motion-2-d2s8.log
echo Motion History 3s Buffer 8
./motion -d 3 -s 8 $1 >motion-1-d3s8.log
./motion -d 3 -s 8 $2 >motion-2-d3s8.log
echo Motion History 5s Buffer 8
./motion -d 5 -s 8 $1 >motion-1-d5s8.log
./motion -d 5 -s 8 $2 >motion-2-d5s8.log
echo Motion History 10s Buffer 8
./motion -d 10 -s 8 $1 >motion-1-d10s8.log
./motion -d 10 -s 8 $2 >motion-2-d10s8.log

echo Motion History 1s Buffer 2 Min-Delta 0.001 Max-Delta 1.0
./motion -m 0.001 -M 1.0 -d 1 $1 >motion-1-d1m001M1.log
./motion -m 0.001 -M 1.0 -d 1 $2 >motion-2-d1m001M1.log
echo Motion History 2s Buffer 2 Min-Delta 0.001 Max-Delta 1.0
./motion -m 0.001 -M 1.0 -d 2 $1 >motion-1-d2m001M1.log
./motion -m 0.001 -M 1.0 -d 2 $2 >motion-2-d2m001M1.log
echo Motion History 3s Buffer 2 Min-Delta 0.001 Max-Delta 1.0
./motion -m 0.001 -M 1.0 -d 3 $1 >motion-1-d3m001M1.log
./motion -m 0.001 -M 1.0 -d 3 $2 >motion-2-d3m001M1.log
echo Motion History 5s Buffer 2 Min-Delta 0.001 Max-Delta 1.0
./motion -m 0.001 -M 1.0 -d 5 $1 >motion-1-d5m001M1.log
./motion -m 0.001 -M 1.0 -d 5 $2 >motion-2-d5m001M1.log
echo Motion History 10s Buffer 2 Min-Delta 0.001 Max-Delta 1.0
./motion -m 0.001 -M 1.0 -d 10 $1 >motion-1-d10m001M1.log
./motion -m 0.001 -M 1.0 -d 10 $2 >motion-2-d10m001M1.log

echo Motion History 1s Buffer 2 Min-Delta 0.001 Max-Delta 3.0
./motion -m 0.001 -M 3.0 -d 1 $1 >motion-1-d1m001M3.log
./motion -m 0.001 -M 3.0 -d 1 $2 >motion-2-d1m001M3.log
echo Motion History 2s Buffer 2 Min-Delta 0.001 Max-Delta 3.0
./motion -m 0.001 -M 3.0 -d 2 $1 >motion-1-d2m001M3.log
./motion -m 0.001 -M 3.0 -d 2 $2 >motion-2-d2m001M3.log
echo Motion History 3s Buffer 2 Min-Delta 0.001 Max-Delta 3.0
./motion -m 0.001 -M 3.0 -d 3 $1 >motion-1-d3m001M3.log
./motion -m 0.001 -M 3.0 -d 3 $2 >motion-2-d3m001M3.log
echo Motion History 5s Buffer 2 Min-Delta 0.001 Max-Delta 3.0
./motion -m 0.001 -M 3.0 -d 5 $1 >motion-1-d5m001M3.log
./motion -m 0.001 -M 3.0 -d 5 $2 >motion-2-d5m001M3.log
echo Motion History 10s Buffer 2 Min-Delta 0.001 Max-Delta 3.0
./motion -m 0.001 -M 3.0 -d 10 $1 >motion-1-d10m001M3.log
./motion -m 0.001 -M 3.0 -d 10 $2 >motion-2-d10m001M3.log

