cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.h
    ${CMAKE_SOURCE_DIR}/src/utils/Debug.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/utils.h
    ${CMAKE_SOURCE_DIR}/src/utils/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.h
    ${CMAKE_SOURCE_DIR}/src/utils/Terminate.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.h
    ${CMAKE_SOURCE_DIR}/src/utils/Assert.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.h
    ${CMAKE_SOURCE_DIR}/src/utils/Logging.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.h
    ${CMAKE_SOURCE_DIR}/src/utils/FileSystem.cpp
    ${CMAKE_SOURCE_DIR}/src/utils/binary_insert_delete.h
)

add_library(${CMAKE_PROJECT_NAME}.Utils ${SOURCE})

target_include_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
)

target_precompile_headers(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/src/pch/pch.cpp ${CMAKE_SOURCE_DIR}/src/pch/pch.h)
target_link_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/glfw/src)
target_link_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/fmt)
target_link_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/lib)
target_link_directories(${CMAKE_PROJECT_NAME}.Utils PRIVATE ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}/vendor/assimp/contrib/zlib)