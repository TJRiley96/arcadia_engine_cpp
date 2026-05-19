cmake_minimum_required(VERSION 3.20)

project(arcadia_engine VERSION 0.1.0 LANGUAGES C CXX)

find_package(OpenGL REQUIRED)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Put the executable in a predictable folder that will also receive runtime DLLs.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/vendor/SDL3-3.4.8")
set(SDL3_IMAGE_ROOT "${CMAKE_SOURCE_DIR}/vendor/SDL3_image-3.4.4")

if(WIN32)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(SDL_ARCH_DIR "x64")
	else()
		set(SDL_ARCH_DIR "x86")
	endif()

	set(SDL3_INCLUDE_DIR "${SDL3_ROOT}/include")
	set(SDL3_IMAGE_INCLUDE_DIR "${SDL3_IMAGE_ROOT}/include")
	set(SDL3_LIB_DIR "${SDL3_ROOT}/lib/${SDL_ARCH_DIR}")
	set(SDL3_IMAGE_LIB_DIR "${SDL3_IMAGE_ROOT}/lib/${SDL_ARCH_DIR}")

	set(SDL3_DLL "${SDL3_LIB_DIR}/SDL3.dll")
	set(SDL3_IMAGE_DLL "${SDL3_IMAGE_LIB_DIR}/SDL3_image.dll")
	set(SDL3_LIB "${SDL3_LIB_DIR}/SDL3.lib")
	set(SDL3_IMAGE_LIB "${SDL3_IMAGE_LIB_DIR}/SDL3_image.lib")

	foreach(REQUIRED_FILE IN ITEMS
		"${SDL3_INCLUDE_DIR}/SDL3/SDL.h"
		"${SDL3_IMAGE_INCLUDE_DIR}/SDL3_image/SDL_image.h"
		"${SDL3_LIB}"
		"${SDL3_IMAGE_LIB}"
		"${SDL3_DLL}"
		"${SDL3_IMAGE_DLL}"
	)
		if(NOT EXISTS "${REQUIRED_FILE}")
			message(FATAL_ERROR "Required SDL file not found: ${REQUIRED_FILE}")
		endif()
	endforeach()

	add_library(SDL3::SDL3 SHARED IMPORTED)
	set_target_properties(SDL3::SDL3 PROPERTIES
		IMPORTED_IMPLIB "${SDL3_LIB}"
		IMPORTED_LOCATION "${SDL3_DLL}"
		INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}"
	)

	add_library(SDL3_image::SDL3_image SHARED IMPORTED)
	set_target_properties(SDL3_image::SDL3_image PROPERTIES
		IMPORTED_IMPLIB "${SDL3_IMAGE_LIB}"
		IMPORTED_LOCATION "${SDL3_IMAGE_DLL}"
		INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INCLUDE_DIR}"
	)
else()
	find_package(SDL3 REQUIRED CONFIG)
	find_package(SDL3_image REQUIRED CONFIG)
endif()

set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/src/glad.c")
set(STB_IMAGE_HEADER "${CMAKE_SOURCE_DIR}/vendor/stb_image.h")
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE WORLD_SRC_FILES CONFIGURE_DEPENDS "src/world/*.cpp")

add_executable(arcadia_engine Main.cpp ${SRC_FILES} ${WORLD_SRC_FILES} ${GLAD_SOURCE} ${STB_IMAGE_HEADER})

target_include_directories(arcadia_engine
	PRIVATE
		"${CMAKE_SOURCE_DIR}/vendor"
		"${CMAKE_SOURCE_DIR}/src/include"
		"${CMAKE_SOURCE_DIR}/src/world/include"
		"${CMAKE_SOURCE_DIR}/src/components/include"
)

if(WIN32)
	target_include_directories(arcadia_engine
		PRIVATE
			"${CMAKE_SOURCE_DIR}/vendor/rapidjson"
			"${CMAKE_SOURCE_DIR}/vendor/SDL3-3.4.8/include"
			"${CMAKE_SOURCE_DIR}/vendor/SDL3_image-3.4.4/include"
	)
endif()

target_link_libraries(arcadia_engine PRIVATE SDL3::SDL3 SDL3_image::SDL3_image OpenGL::GL)

add_custom_target(run_arcadia_engine
	COMMAND "$<TARGET_FILE:arcadia_engine>"
	DEPENDS arcadia_engine
	WORKING_DIRECTORY "$<TARGET_FILE_DIR:arcadia_engine>"
	USES_TERMINAL
)

if(WIN32)
	add_custom_command(TARGET arcadia_engine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${SDL3_DLL}"
			"$<TARGET_FILE_DIR:arcadia_engine>"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${SDL3_IMAGE_DLL}"
			"$<TARGET_FILE_DIR:arcadia_engine>"
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_SOURCE_DIR}/src/assets"
			"$<TARGET_FILE_DIR:arcadia_engine>/assets"
		COMMENT "Copy SDL runtime DLLs next to arcadia_engine.exe"
	)
else()
	add_custom_command(TARGET arcadia_engine POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
			"${CMAKE_SOURCE_DIR}/src/assets"
			"$<TARGET_FILE_DIR:arcadia_engine>/assets"
		COMMENT "Copy assets next to arcadia_engine"
	)
endif()