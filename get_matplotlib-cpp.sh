#!/bin/bash
if [ ! -e ./matplotlib-cpp-starter ]; then
 git clone https://github.com/AtsushiSakai/matplotlib-cpp-starter.git &
 wait
 cd ./matplotlib-cpp-starter
 ./build_and_run.sh
 cd ../
fi