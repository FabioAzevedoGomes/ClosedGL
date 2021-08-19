cmake_minimum_required (VERSION 2.8.12)

project (ClosedGL)

LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Second, for multi-config builds (e.g. msvc)
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/bin)
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_SOURCE_DIR}/lib )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

set (CMAKE_DEBUG_POSTFIX "_d")

find_package(OpenGL REQUIRED)

if(WIN32)
set(COMMON_LIBS ${OPENGL_LIBRARIES} optimized glfw debug glfw)
elseif (UNIX)
set(COMMON_LIBS ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES})
else()
set(COMMON_LIBS)
endif()
set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

add_executable(ClosedGL 
  src/main.cpp 
  src/shaders/ShaderUtils.cpp
  src/3dScene/Model3D.cpp
  src/3dScene/Camera.cpp
  src/3dScene/Scene.cpp
  src/rendering/RenderUtils.cpp
  src/gui/PropertyManager.cpp
  lib/gl3w.c
  lib/imgui/imgui_demo.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imgui.cpp
  lib/imgui/imgui_impl_glfw.cpp
  lib/imgui/imgui_impl_opengl3.cpp
  lib/FileBrowser/ImGuiFileBrowser.cpp
  src/shaders/vertexShader.vert 
  src/shaders/fragmentShader.frag
  )
set_property(TARGET ClosedGL PROPERTY DEBUG_POSTFIX _d)
target_link_libraries(ClosedGL ${COMMON_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GL3W -DGLFW_EXPOSE_NATIVE_WGL")

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_LINUX -")
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

include_directories( include )
include_directories(include/data)
include_directories(include/gui/)
include_directories(include/3dScene)
include_directories(include/shaders)
include_directories(include/rendering)
include_directories(lib)
include_directories(lib/imgui/)
include_directories(lib/FileBrowser)
include_directories(lib/FileBrower/Dirent)
include_directories(lib/glfw/include)

target_link_libraries(ClosedGL imm32.lib)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
