cmake_minimum_required(VERSION 3.26.0)

project(Honeycrisp CXX C)

find_package(Git REQUIRED)

add_subdirectory(vendor)
add_subdirectory(src)


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")  # TODO: How to group targets!????????
    add_compile_definitions(DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}               PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Managers      PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Utils         PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.ECS           PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Components    PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE HNCRSP_DEBUG)
    target_compile_definitions(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE HNCRSP_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME}               PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Managers      PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Graphics      PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Graphics.Core PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Utils         PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.ECS           PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Components    PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.ECSSystems    PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Serializer    PRIVATE -fsanitize=address -g)
    target_compile_options(${CMAKE_PROJECT_NAME}.Scenes        PRIVATE -fsanitize=address -g)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -fsanitize=address -g)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_definitions(NDEBUG)
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
