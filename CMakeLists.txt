cmake_minimum_required(VERSION 3.22)


set(NAME "DisableLevelUpHealing")
set(VERSION 1.0.2)


# ---- Options ----

option(COPY_BUILD "Copy the build output to the Fallout 4 directory." OFF)

# ---- Cache build vars ----

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif()
endmacro()

macro(find_commonlib_path)
	if (CommonLibName AND NOT ${CommonLibName} STREQUAL "")
		# Check extern
		find_path(CommonLibPath
			NAMES
				include/REL/Relocation.h
			PATHS
				extern/${CommonLibName}
				extern/${CommonLibName}/${CommonLibName}
		)
		if (${CommonLibName} STREQUAL "CommonLibName-NOTFOUND")
			#Check path
			set_from_environment(${CommonLibName}Path)
			set(CommonLibName ${${CommonLibName}Path})
		endif()
	endif()
endmacro()

set_from_environment(VCPKG_ROOT)
if (NOT DEFINED VCPKG_ROOT)
	message(
		WARNING
		"Variable VCPKG_ROOT is not set. Continuing without vcpkg."
	)
endif()

set_from_environment(Fallout4Path)
set(CommonLibName "CommonLibF4")
find_commonlib_path("CommonLibF4")

if (DEFINED Fallout4Path)
	message(
		STATUS
		"Building for Fallout 4 at ${Fallout4Path} with ${CommonLibName} at ${CommonLibPath}."
	)
else ()
	message(
		STATUS
		"Building for Fallout 4 with ${CommonLibName} at ${CommonLibPath}."
	)
endif ()

set(Boost_USE_STATIC_RUNTIME OFF CACHE BOOL "")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- Project ----

project(
	${NAME}
	VERSION ${VERSION}
	LANGUAGES CXX
)

# ---- Include guards ----

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(
		FATAL_ERROR
			"In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
	)
endif()

# ---- Project Continuation ----

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

configure_file(
	${ROOT_DIR}/cmake/Version.h.in
	${CMAKE_CURRENT_BINARY_DIR}/include/Version.h
	@ONLY
)

configure_file(
	${ROOT_DIR}/cmake/version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/version.rc
	@ONLY
)

# ---- Globals ----

#[[
add_compile_definitions(
	F4SE_SUPPORT_XBYAK
)
#]]

if (MSVC)
	if (NOT ${CMAKE_GENERATOR} STREQUAL "Ninja")
		add_compile_options(
			/MP	# Build with Multiple Processes
		)
	endif ()
endif ()

set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_DEBUG OFF)

set(Boost_USE_STATIC_LIBS ON)

# ---- Dependencies ----

if (DEFINED CommonLibPath AND NOT ${CommonLibPath} STREQUAL "" AND IS_DIRECTORY ${CommonLibPath})
	add_subdirectory(${CommonLibPath} ${CommonLibName})
else ()
	message(
		FATAL_ERROR
		"Variable ${CommonLibName}Path is not set or in extern/."
	)
endif()

# ---- CMake Build Info ----

include(CMakeLists)
