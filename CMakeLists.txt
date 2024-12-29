cmake_minimum_required(VERSION 3.26.0)

project(Honeycrisp CXX C)

find_package(Git REQUIRED)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    add_compile_definitions(HNCRSP_DEBUG)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_definitions(NDEBUG)
endif()

add_subdirectory(vendor)
add_subdirectory(src)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-O0 -fsanitize=address -g)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address -g)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O3)
    add_link_options(-O3)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    message("MinSizeRel config not supported yet!")
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    message("RelWithDebInfo config not supported yet!")
    # target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE HNCRSP_DEBUG)
    # target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)
    # target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)
    # add_compile_options(-O2)
    # add_link_options(-O2)

endif()
