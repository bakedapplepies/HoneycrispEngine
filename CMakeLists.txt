cmake_minimum_required(VERSION 3.26.0)
# set(CMAKE_CXX_COMPILER )

project(Honeycrisp CXX C)

add_subdirectory(vendor)
add_subdirectory(src)

# TODO: Add options for Debug/Release/MinSizeRel/RelWithDebInfo

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE ASSIMP_BUILD_NO_EXPORT)
target_compile_features(${CMAKE_PROJECT_NAME} PRIVATE cxx_std_20)

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE HNCRSP_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -g -fsanitize=address)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O3)
    add_link_options(-O3)

endif()
