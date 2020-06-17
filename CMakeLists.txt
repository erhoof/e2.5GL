cmake_minimum_required(VERSION 3.16)
project(e2_5GL)

set(CMAKE_CXX_STANDARD 14)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(
        ${OPENGL_INCLUDE_DIRS}
        ${GLUT_INCLUDE_DIRS}
)

add_executable(e2_5GL main.cpp)

target_link_libraries(e2_5GL ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})