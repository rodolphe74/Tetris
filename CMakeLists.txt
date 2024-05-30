cmake_minimum_required(VERSION 3.0)
project(Tetris)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} `-Wdeprecated-declarations`")
# target_compile_options(Tetris PRIVATE -Wdeprecated-declarations)
add_compile_definitions(GL_SILENCE_DEPRECATION)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
set(OpenGL_GL_PREFERENCE LEGACY) # Linux OpenGL
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(FLAC CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED) 
find_library(CoreFoundation_Library CoreFoundation)

set(sharedRes
	resources/Brick3DRegular-nRJR4.ttf
	resources/tetris99main.ogg
	resources/tetris99main2.ogg
	resources/TDrift.ogg
	resources/one.ogg
	resources/two.ogg
	resources/three.ogg
	resources/ready.ogg
	resources/go.ogg
    resources/move.ogg
    resources/asterw.png
    resources/warp.png
    resources/explode.ogg
    resources/rotate.ogg
    resources/move.ogg
    resources/line.ogg
    resources/intro.ogg
    resources/hurry.ogg
    resources/gameover.ogg
    resources/big_square_clear.png
    resources/borderb.png
)


IF(WIN32)
    Message("WINDOWS")
	add_executable(tetris tetris.cpp game.cpp resources.cpp board.cpp animatedBackground.cpp particles.cpp fog.cpp audioStream.cpp menu.cpp ia.cpp gameStatesQueue.cpp fire.cpp)
	target_link_libraries(tetris PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio ${OPENGL_LIBRARY} glm::glm-header-only FLAC::FLAC OpenAL::OpenAL Vorbis::vorbis Vorbis::vorbisfile Vorbis::vorbisenc)
ENDIF()




IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	Message("APPLE")
	add_executable(tetris MACOSX_BUNDLE tetris.cpp game.cpp resources.cpp board.cpp animatedBackground.cpp particles.cpp fog.cpp audioStream.cpp menu.cpp ia.cpp gameStatesQueue.cpp fire.cpp ${sharedRes})

	set_source_files_properties(${sharedRes} PROPERTIES
		MACOSX_PACKAGE_LOCATION Resources
	)

	target_link_libraries(tetris PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio ${OPENGL_LIBRARY} glm::glm-header-only FLAC::FLAC OpenAL::OpenAL Vorbis::vorbis Vorbis::vorbisfile Vorbis::vorbisenc ${CoreFoundation_Library})
ENDIF()
