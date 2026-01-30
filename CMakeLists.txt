cmake_minimum_required(VERSION 3.16)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 14)

add_executable(SolarSystem
        Space/src/Source.cpp
        Dependencies/glad/src/glad.c
)

target_include_directories(SolarSystem PRIVATE
        Dependencies/glad/include
        Dependencies/glm
        Space/src
)

if(WIN32)
    target_include_directories(SolarSystem PRIVATE
            Dependencies/GLFW/include
    )
    target_link_libraries(SolarSystem PRIVATE
            ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/lib-vc2022/glfw3.lib
            opengl32
    )
else()
    find_package(glfw3 REQUIRED)
    find_package(OpenGL REQUIRED)
    target_link_libraries(SolarSystem PRIVATE
            glfw
            OpenGL::GL
            ${CMAKE_DL_LIBS}
    )
endif()

set_target_properties(SolarSystem PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Space"
)

add_custom_command(TARGET SolarSystem POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/Space/Shaders"
        "$<TARGET_FILE_DIR:SolarSystem>/Shaders"
)