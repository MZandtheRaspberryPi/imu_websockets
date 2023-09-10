# imu_websockets
![example workflow](https://github.com/MZandtheRaspberryPi/imu_websockets/actions/workflows/pipeline.yaml/badge.svg)  
A repo that contains a c++ websocket server that can send imu data, serialized via protobuffer.

Intended to be used in other projects via CMake, add_subdirectory(imu_websockets).

```
docker network create compass-net

docker run -it --network=compass-net --name=listener --entrypoint=/tmp/repo/build/imu_websockets_example_listener
docker run -it --network=compass-net --name=broadcaster --entrypoint=/tmp/repo/build/imu_websockets_example_broadcaster

```


Note this branch is an attempt to use shared library for imu_websockets, but to do this we have to recompile protobuffer and that is a big thing. Got pretty far, but had an error finding the protobuffer libraries.

```
root@864e9d4c2dfc:/tmp/repo/build# cmake ..
-- The C compiler identification is GNU 11.4.0
-- The CXX compiler identification is GNU 11.4.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
CMake Deprecation Warning at websocketpp/CMakeLists.txt:5 (cmake_minimum_required):
  Compatibility with CMake < 2.8.12 will be removed from a future version of
  CMake.

  Update the VERSION argument <min> value or use a ...<max> suffix to tell
  CMake that the project does not need compatibility with older versions.


CMake Warning (dev) at websocketpp/CMakeLists.txt:37 (project):
  Policy CMP0048 is not set: project() command manages VERSION variables.
  Run "cmake --help-policy CMP0048" for policy details.  Use the cmake_policy
  command to set the policy and suppress this warning.

  The following variable(s) would be set to empty:

    PROJECT_VERSION
    PROJECT_VERSION_MAJOR
    PROJECT_VERSION_MINOR
    PROJECT_VERSION_PATCH
This warning is for project developers.  Use -Wno-dev to suppress it.

** websocketpp

=========== Used Build Configuration =============

-- ENABLE_CPP11        = ON
-- BUILD_EXAMPLES      = OFF
-- BUILD_TESTS         = OFF

-- WEBSOCKETPP_ROOT    = /tmp/repo/websocketpp
-- WEBSOCKETPP_BIN     = /tmp/repo/build/websocketpp/bin
-- WEBSOCKETPP_LIB     = /tmp/repo/build/websocketpp/lib
-- Install prefix      = /usr/local

-- WEBSOCKETPP_BOOST_LIBS        = 
-- WEBSOCKETPP_PLATFORM_LIBS     = 
-- WEBSOCKETPP_PLATFORM_TLS_LIBS = 

-- OPENSSL_FOUND        = 
-- OPENSSL_INCLUDE_DIR     = 
-- OPENSSL_LIBRARIES = 
-- OPENSSL_VERSION = 

CMake Warning at /usr/share/cmake-3.22/Modules/FindProtobuf.cmake:524 (message):
  Protobuf compiler version 24.3 doesn't match library version 4.24.3
Call Stack (most recent call first):
  CMakeLists.txt:19 (find_package)


CMake Error at /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:230 (message):
  Could NOT find Protobuf (missing: Protobuf_LIBRARIES) (found version
  "4.24.3")
Call Stack (most recent call first):
  /usr/share/cmake-3.22/Modules/FindPackageHandleStandardArgs.cmake:594 (_FPHSA_FAILURE_MESSAGE)
  /usr/share/cmake-3.22/Modules/FindProtobuf.cmake:650 (FIND_PACKAGE_HANDLE_STANDARD_ARGS)
  CMakeLists.txt:19 (find_package)


-- Configuring incomplete, errors occurred!
See also "/tmp/repo/build/CMakeFiles/CMakeOutput.log".
root@864e9d4c2dfc:/tmp/repo/build# exit
exit

```

```
mz@mz-VirtualBox:~/imu_visualizer$ docker build -t oled --progress=plain .
#1 [internal] load build definition from Dockerfile
#1 transferring dockerfile: 861B done
#1 DONE 0.0s

#2 [internal] load .dockerignore
#2 transferring context: 59B done
#2 DONE 0.0s

#3 [internal] load metadata for docker.io/library/ubuntu:22.04
#3 DONE 0.4s

#4 [ 1/20] FROM docker.io/library/ubuntu:22.04@sha256:aabed3296a3d45cede1dc866a24476c4d7e093aa806263c27ddaadbdce3c1054
#4 DONE 0.0s

#5 [internal] load build context
#5 transferring context: 145.15kB 0.1s done
#5 DONE 0.1s

#6 [ 7/20] RUN apt-get install python3-pil python3-dev -y
#6 CACHED

#7 [ 2/20] RUN apt-get update
#7 CACHED

#8 [ 3/20] RUN apt-get install build-essential g++ git-core libi2c-dev i2c-tools lm-sensors cmake unzip wget protobuf-compiler libboost-all-dev  -y
#8 CACHED

#9 [ 4/20] RUN python3 --version
#9 CACHED

#10 [ 5/20] RUN apt-get install python3-pip -y
#10 CACHED

#11 [ 6/20] RUN pip3 install adafruit-circuitpython-ssd1306
#11 CACHED

#12 [ 8/20] RUN pip3 install numpy matplotlib
#12 CACHED

#13 [ 9/20] COPY . /repo
#13 DONE 0.3s

#14 [10/20] RUN chmod +x /repo/imu_visualizer_py/main.py
#14 DONE 0.5s

#15 [11/20] WORKDIR /repo/
#15 DONE 0.1s

#16 [12/20] RUN ls -ltr
#16 0.370 total 52
#16 0.370 -rw-rw-r--  1 root root  299 Sep  9 20:50 DockerfileWeb
#16 0.370 -rwxrwxr-x  1 root root  133 Sep 10 11:53 build_c_lib.sh
#16 0.370 -rw-rw-r--  1 root root  801 Sep 10 12:53 README.md
#16 0.370 drwxrwxr-x  2 root root 4096 Sep 10 14:15 assets
#16 0.370 drwxrwxr-x 16 root root 4096 Sep 10 14:15 eigen
#16 0.370 drwxrwxr-x  2 root root 4096 Sep 10 14:15 http_example
#16 0.370 drwxrwxr-x  1 root root 4096 Sep 10 14:15 imu_visualizer_py
#16 0.370 drwxrwxr-x  2 root root 4096 Sep 10 14:15 src
#16 0.370 drwxrwxr-x  2 root root 4096 Sep 10 14:15 include
#16 0.370 -rw-rw-r--  1 root root  822 Sep 10 16:58 Dockerfile
#16 0.370 -rw-rw-r--  1 root root  454 Sep 10 17:00 CMakeLists.txt
#16 0.370 drwxrwxr-x  7 root root 4096 Sep 10 17:01 imu_websockets
#16 DONE 0.5s

#17 [13/20] RUN mkdir build
#17 DONE 1.4s

#18 [14/20] WORKDIR /repo/build
#18 DONE 0.1s

#19 [15/20] RUN cmake ..
#19 0.602 -- The C compiler identification is GNU 11.4.0
#19 0.662 -- The CXX compiler identification is GNU 11.4.0
#19 0.671 -- Detecting C compiler ABI info
#19 0.756 -- Detecting C compiler ABI info - done
#19 0.763 -- Check for working C compiler: /usr/bin/cc - skipped
#19 0.763 -- Detecting C compile features
#19 0.763 -- Detecting C compile features - done
#19 0.767 -- Detecting CXX compiler ABI info
#19 0.860 -- Detecting CXX compiler ABI info - done
#19 0.867 -- Check for working CXX compiler: /usr/bin/c++ - skipped
#19 0.867 -- Detecting CXX compile features
#19 0.868 -- Detecting CXX compile features - done
#19 0.869 CMake Deprecation Warning at imu_websockets/websocketpp/CMakeLists.txt:5 (cmake_minimum_required):
#19 0.869   Compatibility with CMake < 2.8.12 will be removed from a future version of
#19 0.869   CMake.
#19 0.869 
#19 0.869   Update the VERSION argument <min> value or use a ...<max> suffix to tell
#19 0.869   CMake that the project does not need compatibility with older versions.
#19 0.869 
#19 0.869 
#19 0.869 CMake Warning (dev) at imu_websockets/websocketpp/CMakeLists.txt:37 (project):
#19 0.869   Policy CMP0048 is not set: project() command manages VERSION variables.
#19 0.869   Run "cmake --help-policy CMP0048" for policy details.  Use the cmake_policy
#19 0.869   command to set the policy and suppress this warning.
#19 0.869 
#19 0.869   The following variable(s) would be set to empty:
#19 0.869 
#19 0.869     PROJECT_VERSION
#19 0.869     PROJECT_VERSION_MAJOR
#19 0.869     PROJECT_VERSION_MINOR
#19 0.869     PROJECT_VERSION_PATCH
#19 0.869 This warning is for project developers.  Use -Wno-dev to suppress it.
#19 0.869 
#19 0.870 ** websocketpp
#19 0.871 -- ENABLE_CPP11        = ON
#19 0.871 -- BUILD_EXAMPLES      = OFF
#19 0.871 -- BUILD_TESTS         = OFF
#19 0.871 -- WEBSOCKETPP_ROOT    = /repo/imu_websockets/websocketpp
#19 0.871 -- WEBSOCKETPP_BIN     = /repo/build/imu_websockets/websocketpp/bin
#19 0.871 -- WEBSOCKETPP_LIB     = /repo/build/imu_websockets/websocketpp/lib
#19 0.871 -- Install prefix      = /usr/local
#19 0.871 -- WEBSOCKETPP_BOOST_LIBS        = 
#19 0.871 -- WEBSOCKETPP_PLATFORM_LIBS     = 
#19 0.871 -- WEBSOCKETPP_PLATFORM_TLS_LIBS = 
#19 0.871 -- OPENSSL_FOUND        = 
#19 0.871 -- OPENSSL_INCLUDE_DIR     = 
#19 0.871 -- OPENSSL_LIBRARIES = 
#19 0.871 -- OPENSSL_VERSION = 
#19 0.872 
#19 0.872 =========== Used Build Configuration =============
#19 0.872 
#19 0.872 
#19 0.872 
#19 0.872 
#19 0.872 
#19 1.048 -- Found Protobuf: /usr/lib/x86_64-linux-gnu/libprotobuf.a (found version "3.12.4") 
#19 1.051 -- protheaders: /repo/build/imu_websockets/imu_msgs.pb.h
#19 1.054 -- Configuring done
#19 1.060 -- Generating done
#19 1.062 -- Build files have been written to: /repo/build
#19 DONE 1.2s

#20 [16/20] RUN make
#20 0.470 [ 20%] Running cpp protocol buffer compiler on /repo/imu_websockets/proto/imu_msgs.proto
#20 0.499 [ 40%] Building CXX object imu_websockets/CMakeFiles/imu_websockets_lib.dir/imu_msgs.pb.cc.o
#20 1.463 [ 60%] Linking CXX shared library libimu_websockets_lib.so
#20 1.530 /usr/bin/ld: /usr/lib/x86_64-linux-gnu/libprotobuf.a(arena.o): relocation R_X86_64_TPOFF32 against symbol `_ZN6google8protobuf8internal9ArenaImpl13thread_cache_E' can not be used when making a shared object; recompile with -fPIC
#20 1.530 /usr/bin/ld: failed to set dynamic section sizes: bad value
#20 1.532 collect2: error: ld returned 1 exit status
#20 1.534 make[2]: *** [imu_websockets/CMakeFiles/imu_websockets_lib.dir/build.make:106: imu_websockets/libimu_websockets_lib.so] Error 1
#20 1.535 make[1]: *** [CMakeFiles/Makefile2:158: imu_websockets/CMakeFiles/imu_websockets_lib.dir/all] Error 2
#20 1.535 make: *** [Makefile:136: all] Error 2
#20 ERROR: process "/bin/sh -c make" did not complete successfully: exit code: 2
------
 > [16/20] RUN make:
#20 0.470 [ 20%] Running cpp protocol buffer compiler on /repo/imu_websockets/proto/imu_msgs.proto
#20 0.499 [ 40%] Building CXX object imu_websockets/CMakeFiles/imu_websockets_lib.dir/imu_msgs.pb.cc.o
#20 1.463 [ 60%] Linking CXX shared library libimu_websockets_lib.so
#20 1.530 /usr/bin/ld: /usr/lib/x86_64-linux-gnu/libprotobuf.a(arena.o): relocation R_X86_64_TPOFF32 against symbol `_ZN6google8protobuf8internal9ArenaImpl13thread_cache_E' can not be used when making a shared object; recompile with -fPIC
#20 1.530 /usr/bin/ld: failed to set dynamic section sizes: bad value
#20 1.532 collect2: error: ld returned 1 exit status
#20 1.534 make[2]: *** [imu_websockets/CMakeFiles/imu_websockets_lib.dir/build.make:106: imu_websockets/libimu_websockets_lib.so] Error 1
#20 1.535 make[1]: *** [CMakeFiles/Makefile2:158: imu_websockets/CMakeFiles/imu_websockets_lib.dir/all] Error 2
#20 1.535 make: *** [Makefile:136: all] Error 2
------
Dockerfile:28
--------------------
  26 |     WORKDIR /repo/build
  27 |     RUN cmake ..
  28 | >>> RUN make
  29 |     
  30 |     RUN ls -ltr
--------------------
ERROR: failed to solve: process "/bin/sh -c make" did not complete successfully: exit code: 2

```
