FROM ubuntu:22.04 as build

RUN apt-get update
RUN apt-get install nano git libboost-all-dev cmake build-essential g++ protobuf-compiler -y

COPY . /tmp/repo

WORKDIR /tmp/repo

# RUN protoc -I=/tmp/repo/imu_broadcaster_expected_structure/ --cpp_out=/tmp/repo/imu_broadcaster_expected_structure/proto_msg /tmp/repo/imu_broadcaster_expected_structure/imu_msgs.proto
RUN mkdir build

WORKDIR /tmp/repo/build
RUN cmake ..
RUN make
RUN ls -ltrh
