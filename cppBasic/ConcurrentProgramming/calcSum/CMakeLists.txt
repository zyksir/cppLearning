add_library(libcalcsum INTERFACE)

target_include_directories(libcalcsum INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(libcalcsum INTERFACE cxx_std_11)
target_link_libraries(libcalcsum INTERFACE libwrapper)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()