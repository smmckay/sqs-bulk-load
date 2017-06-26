cmake_minimum_required(VERSION 3.7)
project(sqs_bulk_load)

set(CMAKE_CXX_STANDARD 14)

find_package(AWSSDK REQUIRED)
find_package(Boost 1.58.0 REQUIRED COMPONENTS program_options thread)

set(SOURCE_FILES main.cpp)
add_executable(sqs_bulk_load ${SOURCE_FILES})
target_link_libraries(sqs_bulk_load
        aws-cpp-sdk-sqs
        aws-cpp-sdk-core
        Boost::boost
        Boost::program_options
        Boost::thread
        )