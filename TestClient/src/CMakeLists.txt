cmake_minimum_required(VERSION 3.26.0)
project(${CMAKE_PROJECT_NAME} CXX C)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/TestClient/src/GameEntry.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultScene.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/DefaultSceneTwo.cpp
    ${CMAKE_SOURCE_DIR}/TestClient/src/scenes/SpaceScene.cpp
)

add_executable(${CMAKE_PROJECT_NAME}.TestClient ${SOURCE})

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

target_include_directories(${CMAKE_PROJECT_NAME}.TestClient PRIVATE
    ${CMAKE_SOURCE_DIR}/TestClient/src
    ${CMAKE_SOURCE_DIR}/Engine/include
    ${CMAKE_SOURCE_DIR}/vendor
    ${CMAKE_SOURCE_DIR}/vendor/glfw/include
    ${CMAKE_SOURCE_DIR}/vendor/glm
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include
    ${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp
    ${CMAKE_SOURCE_DIR}/vendor/fmt/include
    ${CMAKE_SOURCE_DIR}/vendor/tracy/public/tracy
)

target_compile_options(${CMAKE_PROJECT_NAME}.TestClient PRIVATE
    ${COMPILE_OPTIONS}    
)

target_link_directories(${CMAKE_PROJECT_NAME}.TestClient PRIVATE
    ${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}
)

target_link_libraries(${CMAKE_PROJECT_NAME}.TestClient PRIVATE
    Honeycrisp
)