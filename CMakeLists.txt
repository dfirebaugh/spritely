cmake_minimum_required(VERSION 3.5)

project("spritely" C)

# Set output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH              ${CMAKE_SOURCE_DIR}/vendor)

# Check if compiler name.
message(${CMAKE_C_COMPILER_ID})

# Set build type definitions.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-D__DEBUG__)
endif (CMAKE_BUILD_TYPE STREQUAL "Debug")
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_definitions(-D__RELEASE__)
endif (CMAKE_BUILD_TYPE STREQUAL "Release")

file(GLOB_RECURSE spritely_hdrs "src/*.h")
file(GLOB_RECURSE spritely_srcs "src/*.c")

include_directories("src/")

find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

include_directories(SDL2 ${SDL2_INCLUDE_DIRS})
include_directories(SDL2TFF ${SDL2TFF_INCLUDE_DIRS})


add_executable(spritely ${spritely_hdrs} ${spritely_srcs})

target_link_libraries(spritely ${SDL2_LIBRARIES})
target_link_libraries(spritely ${SDL2TTF_LIBRARY})
target_link_libraries(spritely -lm)
