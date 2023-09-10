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

find_package(Protobuf REQUIRED)
find_package(absl REQUIRED)

file(GLOB ProtoFiles "${CMAKE_CURRENT_SOURCE_DIR}/proto/*.proto")
PROTOBUF_GENERATE_CPP(ProtoSources ProtoHeaders ${ProtoFiles})

message(STATUS "protheaders: ${ProtoHeaders}")
message(STATUS "absl include dirs ${absl_INCLUDE_DIRS}")
add_library(${PROJECT_NAME}_lib STATIC ${ProtoSources})
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_BINARY_DIR} ${Protobuf_INCLUDE_DIRS} ${absl_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}_lib ${PROTOBUF_LIBRARIES} websocketpp absl::strings)

if(BUILD_IMU_BROADCASTER_EXAMPLES)
     add_executable(${PROJECT_NAME}_example_broadcaster ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_broadcaster.cpp)
     target_link_libraries(${PROJECT_NAME}_example_broadcaster PUBLIC ${PROJECT_NAME}_lib)
     add_executable(${PROJECT_NAME}_example_listener ${CMAKE_CURRENT_SOURCE_DIR}/examples/example_listener.cpp)
     target_link_libraries(${PROJECT_NAME}_example_listener PUBLIC ${PROJECT_NAME}_lib)
endif()