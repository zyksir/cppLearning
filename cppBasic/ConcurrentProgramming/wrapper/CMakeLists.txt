add_library(libwrapper INTERFACE)

set(VSIZE 100 CACHE STRING "Vector Size")

message(STATUS "VSIZE equal to: ${VSIZE}")
target_include_directories(libwrapper INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(libwrapper INTERFACE VSIZE=${VSIZE})
target_compile_features(libwrapper INTERFACE cxx_std_17)