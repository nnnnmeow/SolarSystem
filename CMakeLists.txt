cmake_minimum_required(VERSION 3.16)
project(MilkyWay)

set(CMAKE_CXX_STANDARD 14)

add_executable(MilkyWay
        Space/src/Source.cpp
        Dependencies/glad/src/glad.c
)

target_include_directories(MilkyWay PRIVATE
        Dependencies/glad/include
        Dependencies/glm
        Space/src
)

if(WIN32)
    target_include_directories(MilkyWay PRIVATE
            Dependencies/GLFW/include
    )
    target_link_libraries(MilkyWay PRIVATE
            ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib
            opengl32
    )
else()
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(MilkyWay PRIVATE
            glfw
            OpenGL::GL
            ${CMAKE_DL_LIBS}
    )
endif()

set_target_properties(MilkyWay PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Space"
)

add_custom_command(TARGET MilkyWay POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Space/Shaders"
        "$<TARGET_FILE_DIR:MilkyWay>/Shaders"
)