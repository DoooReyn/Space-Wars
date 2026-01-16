# 设置Cmake最小支持版本
cmake_minimum_required(VERSION 3.10.0)

# 设置项目信息（名称，版本号，使用语言）
project(SpaceWars VERSION 0.1.0 LANGUAGES C CXX)

# 设置可执行文件名称
set(EXE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_NAME})

# 设置C++标准（这里用 C++17）
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置编译选项
if(MSVC)
    add_compile_options(/W4)
elseif(APPLE)
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/dist)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/dist)

# 查找并载入Cmake预设
find_package(SDL2 REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# 添加可执行文件
add_executable(${EXE_NAME}
    "src/main.cpp"
    "src/Game.cpp"
    "src/SceneMain.cpp"
)

# 链接库
target_link_libraries(${EXE_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_mixer::SDL2_mixer SDL2_ttf::SDL2_ttf)
