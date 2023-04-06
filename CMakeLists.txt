cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

project(game)

set(SOURCES 
    main.cpp
    src/Game.cpp
    src/TextureManager.cpp
    src/TickTimer.cpp
    src/Scene.cpp
)

set(SDL2_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/SDL2/include")

set(SDL2_LIBRARY_DIR "${PROJECT_SOURCE_DIR}/SDL2/lib")


include_directories(${SDL2_INCLUDE_DIR}
    ${PROJECT_SOURCE_DIR}/include
    
    ${PROJECT_SOURCE_DIR}/entt
)

#temp
include_directories(${PROJECT_SOURCE_DIR}/src)


add_executable(game ${SOURCES})

target_link_libraries(game 
    mingw32 
    ${SDL2_LIBRARY_DIR}/libSDL2main.a 
    ${SDL2_LIBRARY_DIR}/libSDL2.dll.a 
    ${SDL2_LIBRARY_DIR}/libSDL2_image.dll.a 
    ${SDL2_LIBRARY_DIR}/libSDL2_ttf.dll.a 
    -mwindows
    -mconsole
)