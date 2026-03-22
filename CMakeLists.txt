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

add_subdirectory(src)

include(CTest)
add_subdirectory(tests)
