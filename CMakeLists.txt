cmake_minimum_required(VERSION 3.5)
project(sqs_bulk_load)

set(CMAKE_CXX_STANDARD 11)
set(BUILD_SHARED_LIBS OFF)
set(ENABLE_UNITY_BUILD ON)

add_subdirectory(lib/aws-sdk-cpp EXCLUDE_FROM_ALL)
find_package(Boost 1.54.0 REQUIRED COMPONENTS program_options thread system)

set(SOURCE_FILES src/main.cpp)
add_executable(sqs_bulk_load ${SOURCE_FILES})
target_include_directories(sqs_bulk_load PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(sqs_bulk_load
        aws-cpp-sdk-sqs
        aws-cpp-sdk-core
        ${Boost_PROGRAM_OPTIONS_LIBRARY}
        ${Boost_THREAD_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        )