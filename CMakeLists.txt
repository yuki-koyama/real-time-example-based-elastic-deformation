cmake_minimum_required(VERSION 3.1)
project(real-time-example-based-elastic-deformation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

# This should be set OFF if this app is built for non-retina displays
option(BUILD_FOR_RETINA_DISPLAY "Build for retina display (e.g., recent MacBook)" ON)

find_package(OpenMesh REQUIRED)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# This is not very elegant way to include external libraries...
file(GLOB klib_sources ${CMAKE_CURRENT_SOURCE_DIR}/include/KLIB/*.cpp)

add_executable(app ${sources} ${headers} ${klib_sources})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/include)
target_link_libraries(app
  OpenMeshCore
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtet.a
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

# Bug fixing in AntTweakBar for macOS
# sed -e 's/GLchar\* \*string/GLchar\* const \*string/' ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/LoadOGLCore.h
# sed -e 's/GLvoid\* \*indices/GLvoid\* const \*indices/' ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/LoadOGLCore.h

add_custom_command(TARGET app PRE_BUILD COMMAND make -C ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src -f ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/Makefile.osx)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:app>/shaders)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>)
