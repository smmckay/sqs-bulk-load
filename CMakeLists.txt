cmake_minimum_required(VERSION 3.5)
project(sqs_bulk_load)

set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "build AWS SDK shared" FORCE)
set(ENABLE_UNITY_BUILD ON CACHE BOOL "unity build for AWS SDK" FORCE)

add_subdirectory(lib/aws-sdk-cpp EXCLUDE_FROM_ALL)
find_package(Boost 1.54.0 REQUIRED COMPONENTS program_options thread system)

set(SOURCE_FILES src/main.cpp)
add_executable(sqs_bulk_load ${SOURCE_FILES})
target_link_libraries(sqs_bulk_load
        aws-cpp-sdk-sqs
        aws-cpp-sdk-core
        Boost::boost
        Boost::program_options
        Boost::thread
        )
