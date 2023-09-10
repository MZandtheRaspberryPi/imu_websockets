#!/bin/bash

set -eux o pipefail

apt-get install wget tar g++ cmake -y

mkdir /abseil
wget -O /abseil/20230802.0.tar.gz https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.tar.gz
tar -zxf /abseil/20230802.0.tar.gz --directory /abseil/
cd /abseil/abseil-cpp-20230802.0/
mkdir build && cd build
cmake -E env CXXFLAGS="-fPIC" cmake .. -DCMAKE_INSTALL_PREFIX=/abseil/CMakeProject/install
cmake --build . --target install