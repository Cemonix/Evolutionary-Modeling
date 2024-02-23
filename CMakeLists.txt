cmake_minimum_required(VERSION 3.5)
project(EvolutionaryModeling C)

set(CMAKE_C_STANDARD 23)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            DOWNLOAD_EXTRACT_TIMESTAMP OFF
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)

    if (NOT raylib_POPULATED)
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # Don't build the supplied examples
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # Don't build the supplied games
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB SOURCES "src/*.c")
add_executable(${PROJECT_NAME} ${SOURCES} main.c)
target_link_libraries(${PROJECT_NAME} raylib)
