cmake_minimum_required(VERSION 3.20)

project(Matrix_Calculator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(matrix_calculator src/main.cpp src/matrix.cpp)

enable_testing()

add_subdirectory(tests)
