cmake_minimum_required(VERSION 3.20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # pour zed

project(SimulationSDR
    VERSION 1.0.0
    LANGUAGES CXX
)

# Forcer le standard C++ (ex: C++20)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Désactive les extensions spécifiques aux compilateurs (ex: gnu++20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # pour zed

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

option(ENABLE_STATS "Enable compilation of stats code" OFF)

add_subdirectory(src)

include(CTest)
add_subdirectory(tests)

include(FetchContent)

FetchContent_Declare(
    CLI11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG        main
)

FetchContent_MakeAvailable(CLI11)
