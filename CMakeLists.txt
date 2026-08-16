cmake_minimum_required(VERSION 4.4)

project(MyProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE INCLUDE_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*"
)

add_executable(MyProject
    src/main.cpp
    ${INCLUDE_FILES}
)

target_include_directories(MyProject PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)