cmake_minimum_required(VERSION 3.1)
project(real-time-example-based-elastic-deformation)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_BUILD_TYPE Release)

find_package(OpenMesh REQUIRED)

file(GLOB sources ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB headers ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# This is not very elegant way...
file(GLOB klib_sources ${CMAKE_CURRENT_SOURCE_DIR}/include/KLIB/*.cpp)
file(GLOB at_sources ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/*.h)

add_executable(app ${sources} ${headers} ${klib_sources} ${at_sources})
target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/include)
target_compile_definitions(app PRIVATE _MACOSX)
target_compile_options(app PUBLIC -ObjC++)
target_link_libraries(app
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libtet.a
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libAntTweakBar.dylib
  OpenMeshCore
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEW.a
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/libGLEWmx.a
  "-framework GLUT"
  "-framework OpenGL"
)
add_custom_command(TARGET app PRE_BUILD COMMAND make -f ${CMAKE_CURRENT_SOURCE_DIR}/external/AntTweakBar/src/Makefile.osx)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:app>/shaders)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:app>/data)
add_custom_command(TARGET app POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.dylib $<TARGET_FILE_DIR:app>/)
