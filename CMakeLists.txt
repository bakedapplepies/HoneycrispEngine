cmake_minimum_required(VERSION 3.26.0)
project(LearnOpenGL CXX C)

add_subdirectory(dependencies)
add_subdirectory(src)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -O3)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -O3)

endif()
