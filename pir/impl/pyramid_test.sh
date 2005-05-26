#!/bin/sh

#Options:
#./pyramid 
#  -t | --threshold      <double> threshold for group movement, default=1.000000
#  -f | --feature-points <path>   file containing feature points, no default
#  -v | --visualize               display windows
#  -h | --help                    show help
#<path>                           recording directory, default = .

echo threshold: default fps: f1
./pyramid -f ./f1 $1 >pyramid-f1-1.log
./pyramid -f ./f1 $2 >pyramid-f1-2.log

echo threshold: default fps: f2
./pyramid -f ./f2 $1 >pyramid-f2-1.log
./pyramid -f ./f2 $2 >pyramid-f1-2.log

echo threshold: 0.5 fps: f1
./pyramid -t 0.5 -f ./f1 $1 >pyramid-t05-f1-1.log
./pyramid -t 0.5 -f ./f1 $2 >pyramid-t05-f1-2.log

echo threshold: 0.5 fps: f2
./pyramid -t 0.5 -f ./f2 $1 >pyramid-t05-f2-1.log
./pyramid -t 0.5 -f ./f2 $2 >pyramid-t05-f2-2.log

echo threshold: 5.0 fps: f1
./pyramid -t 5.0 -f ./f1 $1 >pyramid-t50-f1-1.log
./pyramid -t 5.0 -f ./f1 $2 >pyramid-t50-f1-2.log

echo threshold: 5.0 fps: f2
./pyramid -t 5.0 -f ./f2 $1 >pyramid-t50-f2-1.log
./pyramid -t 5.0 -f ./f2 $2 >pyramid-t50-f2-2.log

echo threshold: 10.0 fps: f1
./pyramid -t 10.0 -f ./f1 $1 >pyramid-t100-f1-1.log
./pyramid -t 10.0 -f ./f1 $2 >pyramid-t100-f1-2.log

echo threshold: 10.0 fps: f2
./pyramid -t 10.0 -f ./f2 $1 >pyramid-t100-f2-1.log
./pyramid -t 10.0 -f ./f2 $2 >pyramid-t100-f2-2.log
