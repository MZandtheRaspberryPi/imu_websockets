# imu_websockets
![example workflow](https://github.com/MZandtheRaspberryPi/imu_websockets/actions/workflows/pipeline.yaml/badge.svg)  
A repo that contains a c++ websocket server that can send imu data, serialized via protobuffer.

Intended to be used in other projects via CMake, add_subdirectory(imu_websockets).

```
docker network create compass-net

docker run -it --network=compass-net --name=listener --entrypoint=/tmp/repo/build/imu_websockets_example_listener
docker run -it --network=compass-net --name=broadcaster --entrypoint=/tmp/repo/build/imu_websockets_example_broadcaster

```


Note we now use a use shared library for imu_websockets, but to do this we have to recompile protobuffer and that is a big thing.
