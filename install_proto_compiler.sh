#!/bin/bash

set -eux o pipefail

# apt-get install wget tar g++ cmake -y

# mkdir /abseil
# wget -O /abseil/20230802.0.tar.gz https://github.com/abseil/abseil-cpp/archive/refs/tags/20230802.0.tar.gz
# tar -zxf /abseil/20230802.0.tar.gz --directory /abseil/
# cd /abseil/abseil-cpp-20230802.0/
# mkdir build && cd build
# cmake .. -DCMAKE_INSTALL_PREFIX=~/abseil/CMakeProject/install
# cmake --build . --target install

mkdir /protobuf/
wget -O /protobuf/protobuf-24.3.tar.gz https://github.com/protocolbuffers/protobuf/releases/download/v24.3/protobuf-24.3.tar.gz
tar -zxf /protobuf/protobuf-24.3.tar.gz --directory /protobuf
cd /protobuf/protobuf-24.3
mkdir build && cd build
cmake -E env CXXFLAGS="-fPIC" cmake .. -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_LIBPROTOC=ON -Dprotobuf_ABSL_PROVIDER=package -Dabsl_DIR=/abseil/CMakeProject/install/lib/cmake/absl -DBUILD_SHARED_LIBS=ON -Dprotobuf_BUILD_SHARED_LIBS=ON -Dprotobuf_INSTALL=ON
cmake --build . --target install
