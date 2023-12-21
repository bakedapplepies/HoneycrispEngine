cmake_minimum_required(VERSION 3.26.0)
project(LearnOpenGL.Vendors CXX C)

add_subdirectory(glm)
add_subdirectory(assimp)

# message("Building assimp")
# add_custom_command(OUTPUT "" PRE_BUILD
#     # COMMAND "cd ${CMAKE_SOURCE_DIR}/vendor/assimp"
#     # COMMAND ${CMAKE_COMMAND} -E cmake . -G "Ninja"
#     # COMMAND ${CMAKE_COMMAND} -E cd ../../
#     COMMAND ${CMAKE_COMMAND} "message(aldskjfalkdf)"
# )
# execute_process(COMMAND cd ${CMAKE_SOURCE_DIR}/vendor/assimp/)
# # execute_process(COMMAND cmake . -G "Ninja")
# # execute_process(COMMAND cd ../../)

if (NOT EXISTS "${CMAKE_SOURCE_DIR}/vendor/assimp/include/assimp/config.h")
    message("Building assimp")
    execute_process(
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/assimp
        COMMAND cmake . -G "Ninja"
    )
endif()