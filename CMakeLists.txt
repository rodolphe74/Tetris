cmake_minimum_required(VERSION 3.0)
project(Tetris)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY) # Linux OpenGL
find_package(OpenGL REQUIRED)

add_executable(tetris tetris.cpp board.cpp animatedBackground.cpp particles.cpp fog.cpp audioStream.cpp menu.cpp ia.cpp gameStatesQueue.cpp)

target_link_libraries(tetris PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio ${OPENGL_LIBRARY})