cmake_minimum_required(VERSION 3.5)
project(adv-games-dev)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


SET (EXE_LOCATION "${CMAKE_BINARY_DIR}/bin/")
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXE_LOCATION})

find_package(OpenGL REQUIRED)


file(GLOB_RECURSE SOURCE_FILES adv-games-dev/*.cpp adv-games-dev/*.h)
link_directories("${CMAKE_SOURCE_DIR}/lib/x64")
add_executable(adv-games-dev ${SOURCE_FILES})



target_include_directories(adv-games-dev PUBLIC ${OPENGL_INCLUDE_DIRS})
target_include_directories(adv-games-dev PUBLIC "include/")
#include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(adv-games-dev ${OPENGL_LIBRARIES})
target_link_libraries(adv-games-dev
"glew32" 
"SDL2main"
"SDL2"
"SDL2_ttf"
"SDL2_image"
"SDL2test"
"assimp-vc140-mt"
)



add_custom_target(copy_resources ALL
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/objects" "${CMAKE_BINARY_DIR}"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/shader" "${CMAKE_BINARY_DIR}"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/Images" "${CMAKE_BINARY_DIR}"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
		  "${PROJECT_SOURCE_DIR}/Font" "${CMAKE_BINARY_DIR}"
          COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/dll" "${EXE_LOCATION}/Debug"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${CMAKE_SOURCE_DIR}/dll" "${EXE_LOCATION}/Release"

		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/objects" "${EXE_LOCATION}/Release"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/shader" "${EXE_LOCATION}/Release"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
          "${PROJECT_SOURCE_DIR}/Images" "${EXE_LOCATION}/Release"
		  COMMAND ${CMAKE_COMMAND} -E copy_directory
		  "${PROJECT_SOURCE_DIR}/Font" "${EXE_LOCATION}/Release"
          )

add_dependencies(adv-games-dev  copy_resources)