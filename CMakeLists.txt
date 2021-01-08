cmake_minimum_required (VERSION 2.8)
project (tinyrenderer)

#版本号
set (TINYRENDER_VERSION_MAJOR 1)
set (TINYRENDER_VERSION_MAJOR 0)
#添加配置文件
configure_file (
  "${PROJECT_SOURCE_DIR}/TinyRenderConfig.h.in"
  "${PROJECT_BINARY_DIR}/TinyRenderConfig.h"
  )
 
#配置文件会写入BINARY_DIR中，添加到include的搜索路径中
include_directories("${PROJECT_BINARY_DIR}")

# 添加头文件的查找目录  
include_directories(${PROJECT_SOURCE_DIR}/include)  
include_directories(${PROJECT_SOURCE_DIR}/imgui)  
include_directories(${PROJECT_SOURCE_DIR}/imgui/libs/gl3w)  
include_directories(${PROJECT_SOURCE_DIR}/imgui/libs/glfw/include)  

file(GLOB SOURCES *.h *.cpp)



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  
add_executable(${PROJECT_NAME} ${SOURCES})

