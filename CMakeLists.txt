cmake_minimum_required(VERSION 3.1)
project(real-time-example-based-elastic-deformation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

# ------------------------------------------------------------------------------
# Optioins
# ------------------------------------------------------------------------------

# This should be set OFF if this app is built for non-retina displays
option(BUILD_FOR_RETINA_DISPLAY "Build for retina display (e.g., recent MacBook)" ON)

# ------------------------------------------------------------------------------
# External dependency: OpenMesh
# ------------------------------------------------------------------------------
find_package(OpenMesh REQUIRED)

# ------------------------------------------------------------------------------
# External dependency: Eigen3
# ------------------------------------------------------------------------------
find_package(Eigen3 REQUIRED)
if((NOT TARGET Eigen3::Eigen) AND (DEFINED EIGEN3_INCLUDE_DIR))
	# Eigen 3.0, 3.1, 3.2 do not provide the target named Eigen3::Eigen
	add_library(AliasEigen3 INTERFACE)
	target_include_directories(AliasEigen3 INTERFACE ${EIGEN3_INCLUDE_DIR})
	add_library(Eigen3::Eigen ALIAS AliasEigen3)
endif()

# ------------------------------------------------------------------------------
# Dependency: TetGen
# ------------------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/tetgen)

# This is not elegant
target_include_directories(tetgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external/tetgen)

# ------------------------------------------------------------------------------
# Main app
# ------------------------------------------------------------------------------

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# This is not very elegant way to include external libraries...
file(GLOB klib_sources ${CMAKE_CURRENT_SOURCE_DIR}/include/KLIB/*.cpp)
file(GLOB tinyxml_sources ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml/src/*.cpp)

add_executable(app ${sources} ${headers} ${klib_sources} ${tinyxml_sources})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/include ${CMAKE_CURRENT_SOURCE_DIR}/external/tinyxml/include)
target_link_libraries(app
  OpenMeshCore
  Eigen3::Eigen
  tetgen
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEWmx.a
  ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/lib/libAntTweakBar.a
  "-framework GLUT"
  "-framework OpenGL"
  "-framework AppKit"
)
if(BUILD_FOR_RETINA_DISPLAY)
	target_compile_definitions(app PRIVATE BUILD_FOR_RETINA_DISPLAY)
endif()

# ------------------------------------------------------------------------------
# Dependency: AntTweakBar
# ------------------------------------------------------------------------------

# Bug fixing in AntTweakBar for macOS
# sed -e 's/GLchar\* \*string/GLchar\* const \*string/' ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/LoadOGLCore.h
# sed -e 's/GLvoid\* \*indices/GLvoid\* const \*indices/' ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/LoadOGLCore.h

add_custom_command(TARGET app PRE_BUILD COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src -f ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/Makefile.osx)

# ------------------------------------------------------------------------------
# Assets
# ------------------------------------------------------------------------------

add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:app>/shaders)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>)
