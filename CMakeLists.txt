cmake_minimum_required(VERSION 3.16)

project(imu_websockets VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option (BUILD_IMU_BROADCASTER_EXAMPLES "Build examples of lib" FALSE)

set(CMAKE_FIND_LIBRARY_SUFFIXES .a)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/websocketpp)
add_library(websocketpp INTERFACE)
target_include_directories(websocketpp INTERFACE ${websocketpp_SOURCE_DIR})

find_package (Protobuf REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

message(STATUS "protheaders: ${ProtoHeaders}")

add_library(${PROJECT_NAME}_lib STATIC ${ProtoSources})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${PROJECT_NAME}_lib ${PROTOBUF_LIBRARIES} websocketpp)


if (BUILD_IMU_BROADCASTER_EXAMPLES)
     add_executable(${PROJECT_NAME}_example ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_broadcaster.cpp)
     target_link_libraries(${PROJECT_NAME}_example PUBLIC ${PROJECT_NAME}_lib)
endif()