cmake_minimum_required(VERSION 3.16)

project(imu_websockets VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_IMU_BROADCASTER_EXAMPLES "Build examples of lib" FALSE)

set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
SET(GCC_COVERAGE_COMPILE_FLAGS "-fPIC -Dprotobuf_BUILD_SHARED_LIBS=ON")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/websocketpp)
add_library(websocketpp INTERFACE)
target_include_directories(websocketpp INTERFACE ${websocketpp_SOURCE_DIR})

find_package(Protobuf REQUIRED CONFIG)
find_package(absl REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")

# PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})
message(STATUS "protheaders: ${ProtoHeaders}")
message(STATUS "absl include dirs ${absl_INCLUDE_DIRS}")
add_library(${PROJECT_NAME}_lib SHARED ${ProtoSources})

set(PROTO_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
protobuf_generate(
     LANGUAGE cpp
     TARGET ${PROJECT_NAME}_lib
     PROTOS ${ProtoFiles}
     PROTOC_OUT_DIR "${PROTO_BINARY_DIR}")
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ${Protobuf_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/proto ${absl_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib ${PROTOBUF_LIBRARIES} absl::base absl::synchronization absl::strings protobuf::libprotobuf websocketpp)

if(BUILD_IMU_BROADCASTER_EXAMPLES)
     add_executable(${PROJECT_NAME}_example_broadcaster ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_broadcaster.cpp)
     target_link_libraries(${PROJECT_NAME}_example_broadcaster PUBLIC ${PROJECT_NAME}_lib)
     add_executable(${PROJECT_NAME}_example_listener ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_listener.cpp)
     target_link_libraries(${PROJECT_NAME}_example_listener PUBLIC ${PROJECT_NAME}_lib)
endif()