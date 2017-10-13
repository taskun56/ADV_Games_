cmake_minimum_required(VERSION 3.5)

project(adv-games-dev)


set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)


find_package(OpenGL REQUIRED)



add_executable(adv-games-dev adv-games-dev/main.cpp

)




include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(adv-games-dev ${OPENGL_LIBRARIES})