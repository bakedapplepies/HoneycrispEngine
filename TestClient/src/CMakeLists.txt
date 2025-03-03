cmake_minimum_required(VERSION 3.26.0)
project(TestClient CXX C)

# TODO: remove this
add_compile_definitions(
    GLM_FORCE_SWIZZLE
    GLM_ENABLE_EXPERIMENTAL
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/TestClient/src/GameEntry.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/Setup.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultSceneTwo.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/SpaceScene.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE})

# -ffile-prefix-map=${CMAKE_SOURCE_DIR}=. messes up the debugger
set(COMPILE_OPTIONS
    # -ffile-prefix-map=${CMAKE_SOURCE_DIR}=.
    # -fdebug-prefix-map=${CMAKE_SOURCE_DIR}=.
    -Wall
    -Wextra
    -pedantic
    -Wno-unused-parameter
    -Wno-unused-variable
    -Wno-unused-private-field
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/TestClient/src
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/Engine/vendor
    ${CMAKE_SOURCE_DIR}/Engine/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/Engine/vendor/glm
    ${CMAKE_SOURCE_DIR}/Engine/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/Engine/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/Engine/vendor/fmt/include
    ${CMAKE_SOURCE_DIR}/Engine/vendor/tracy/public/tracy
)

target_compile_options(${PROJECT_NAME} PRIVATE
    ${COMPILE_OPTIONS}    
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_BINARY_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Honeycrisp
)

# Build-type configurations
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    add_compile_options(-O0 -fsanitize=address -g)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address -g)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_compile_options(-O3)
    add_link_options(-O3)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "MinSizeRel")
    message("MinSizeRel config not supported yet!")
    
elseif(${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
    message("RelWithDebInfo config not supported yet!")

endif()