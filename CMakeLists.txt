cmake_minimum_required(VERSION 3.25)
project(FractalGenerator)

set(CMAKE_CXX_STANDARD 23)

set(OpenMP_CXX "${CMAKE_CXX_COMPILER}")
set(OpenMP_CXX_FLAGS "-fopenmp")
set(OpenMP_CXX_LIB_NAMES "libomp" "libiomp5")
set(OpenMP_libomp_LIBRARY libomp)
set(OpenMP_libiomp5_LIBRARY libiomp5)

# Компилировать все зависимости внутрь
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

#OPENMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
set(RAYLIB_VERSION 4.5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

add_executable(FractalGenerator main.cpp fractal/Fractal.hpp)

target_link_libraries(${PROJECT_NAME} raylib)