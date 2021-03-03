#!/bin/bash
#
#    * File     : build.sh
#    * Author   : *
#    * Mail     : *
#    * Creation : Thu 18 Feb 2021 09:20:33 PM CST

rm -rf build
rm -rf lib

mkdir build lib

cd `pwd`/build && 
    cmake .. && 
    make 

cd ..

echo "build complete!"


