cmake_minimum_required(VERSION 3.20)
project(QtBoilerplate)

set(CMAKE_CXX_STANDARD 14)

# Qt setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)

message("res folder: ${CMAKE_BINARY_DIR}/resources/")
add_executable(QtBoilerplate main.cpp TTTGUI.hpp Game.h Game.cpp)
target_link_libraries(QtBoilerplate PUBLIC Qt5::Core Qt5::Widgets)
target_compile_definitions(QtBoilerplate PRIVATE RES_FOLDER="${CMAKE_BINARY_DIR}/resources/")

add_library(Catch2 INTERFACE)
target_include_directories(Catch2 INTERFACE "../common/include")

add_executable(BoilerplateTest test.cpp)
target_link_libraries(BoilerplateTest Catch2)


enable_testing()
add_test(NAME "BoilerplateTest" COMMAND BoilerplateTest)