cmake_minimum_required(VERSION 3.24)

project(arcadia_engine LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenGL context request defaults (can be overridden by presets).
set(ARCADIA_GL_CONTEXT_MAJOR "3" CACHE STRING "Requested OpenGL major version")
set(ARCADIA_GL_CONTEXT_MINOR "3" CACHE STRING "Requested OpenGL minor version")
set(ARCADIA_GL_CONTEXT_PROFILE "CORE" CACHE STRING "Requested OpenGL profile (CORE or COMPATIBILITY)")

# Keep dependencies self-contained and avoid requiring SDL installs on host systems.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Disable SDL3 tests" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "Disable SDL3 test library" FORCE)
set(SDL_INSTALL OFF CACHE BOOL "Disable SDL3 install target" FORCE)
set(SDL_SHARED OFF CACHE BOOL "Disable SDL3 shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Enable SDL3 static library" FORCE)

set(SDLIMAGE_SAMPLES OFF CACHE BOOL "Disable SDL3_image samples" FORCE)
set(SDLIMAGE_TESTS OFF CACHE BOOL "Disable SDL3_image tests" FORCE)
set(SDLIMAGE_INSTALL OFF CACHE BOOL "Disable SDL3_image install target" FORCE)
set(SDLIMAGE_VENDORED OFF CACHE BOOL "Use normal dependency detection for SDL3_image" FORCE)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3-3.4.8" "${CMAKE_BINARY_DIR}/vendor/SDL3")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/vendor/SDL3_image-3.4.4" "${CMAKE_BINARY_DIR}/vendor/SDL3_image")

file(GLOB_RECURSE ARC_ENGINE_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/components/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/world/*.cpp"
)

add_executable(${PROJECT_NAME}
	"${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
	${ARC_ENGINE_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/components/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/world/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor"
)

find_package(OpenGL REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE
	SDL3::SDL3
	SDL3_image::SDL3_image
	OpenGL::GL
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	ARCADIA_GL_CONTEXT_MAJOR=${ARCADIA_GL_CONTEXT_MAJOR}
	ARCADIA_GL_CONTEXT_MINOR=${ARCADIA_GL_CONTEXT_MINOR}
)

if(ARCADIA_GL_CONTEXT_PROFILE STREQUAL "COMPATIBILITY")
	target_compile_definitions(${PROJECT_NAME} PRIVATE ARCADIA_GL_PROFILE_COMPATIBILITY=1)
else()
	target_compile_definitions(${PROJECT_NAME} PRIVATE ARCADIA_GL_PROFILE_CORE=1)
endif()

if(WIN32)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		imm32
		setupapi
		version
		winmm
	)
elseif(UNIX AND NOT APPLE)
	target_link_libraries(${PROJECT_NAME} PRIVATE
		dl
		m
		pthread
	)
endif()

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_CURRENT_SOURCE_DIR}/src/assets"
	"$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
	COMMENT "Copying assets folder to executable directory"
)

if(WIN32)
	# Copy MinGW runtime DLLs required by GCC-built executables.
	if(MINGW)
		get_filename_component(MINGW_BIN_DIR "${CMAKE_CXX_COMPILER}" DIRECTORY)
		foreach(MINGW_RUNTIME_DLL libstdc++-6.dll libgcc_s_seh-1.dll libwinpthread-1.dll)
			if(EXISTS "${MINGW_BIN_DIR}/${MINGW_RUNTIME_DLL}")
				add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different
					"${MINGW_BIN_DIR}/${MINGW_RUNTIME_DLL}"
					"$<TARGET_FILE_DIR:${PROJECT_NAME}>"
					COMMENT "Copying ${MINGW_RUNTIME_DLL}"
				)
			endif()
		endforeach()
	endif()
endif()

