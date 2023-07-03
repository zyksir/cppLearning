cmake_minimum_required(VERSION 3.14)
project(calcSum_app)
set(CMAKE_CXX_STANDARD 11)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

add_subdirectory(wrapper)
add_subdirectory(calcSum)
add_executable(calcSum_app main.cpp)
target_link_libraries(calcSum_app libcalcsum)