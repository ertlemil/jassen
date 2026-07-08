cmake_minimum_required(VERSION 3.28)
project(Jassen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG        v3.0.4
)
FetchContent_MakeAvailable(pybind11)

add_executable(Jassen src/main.cpp)
pybind11_add_module(jass_engine src/Wrapper.cpp)

target_include_directories(jass_engine PRIVATE src)
target_include_directories(Jassen PRIVATE src)
