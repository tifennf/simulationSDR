include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        v1.14.0
)
FetchContent_MakeAvailable(googletest)


add_executable(tests_simulationSDR test_transmitter.cpp)


target_sources(tests_simulationSDR PRIVATE ${PROJECT_SOURCE_DIR}/src/transmitter.cpp)


target_include_directories(tests_simulationSDR PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(tests_simulationSDR PRIVATE gtest_main)


include(GoogleTest)
gtest_discover_tests(tests_simulationSDR)
