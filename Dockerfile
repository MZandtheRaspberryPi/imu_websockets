FROM ubuntu:22.04 as build

RUN apt-get update
RUN apt-get install nano git libboost-all-dev cmake build-essential g++ i2c-tools libi2c-dev wget -y

COPY ./install_abseil.sh /tmp/install_abseil.sh
RUN chmod +x /tmp/install_abseil.sh
RUN /tmp/install_abseil.sh

COPY ./install_proto_compiler.sh /tmp/install_proto_compiler.sh
RUN chmod +x /tmp/install_proto_compiler.sh
RUN /tmp/install_proto_compiler.sh

COPY . /tmp/repo
WORKDIR /tmp/repo
# # RUN protoc -I=/tmp/repo/imu_broadcaster_expected_structure/ --cpp_out=/tmp/repo/imu_broadcaster_expected_structure/proto_msg /tmp/repo/imu_broadcaster_expected_structure/imu_msgs.proto
RUN mkdir build

WORKDIR /tmp/repo/build
RUN cmake -D BUILD_IMU_BROADCASTER_EXAMPLES=TRUE -DProtobuf_LIBRARIES=/usr/local/lib/libprotobuf.so -Dprotobuf_ABSL_PROVIDER='package' -Dabsl_DIR=/abseil/CMakeProject/install/lib/cmake/absl ..
RUN make
RUN ls -ltrh
