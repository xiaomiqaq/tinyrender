cmake_minimum_required (VERSION 2.8)
project (tinyrenderer)

#版本号
set (TINYRENDER_VERSION_MAJOR 1)
set (TINYRENDER_VERSION_MAJOR 0)
#添加配置文件
set(HEADERS
  
)


# 添加头文件的查找目录  
include_directories(${PROJECT_SOURCE_DIR}/include)  
include_directories(${PROJECT_SOURCE_DIR}/imgui)  


file(GLOB SOURCES *.h *.cpp)



SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)  
add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} )

