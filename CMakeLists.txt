cmake_minimum_required(VERSION 3.26)
project(yumesdl)

set(CMAKE_CXX_STANDARD 20)

set(SDL_SHARED ON)

add_subdirectory(glm)

include(FetchContent)
FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-2.28.5
)
FetchContent_MakeAvailable(SDL)

FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG release-2.8.2
)
FetchContent_MakeAvailable(SDL_image)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/config.h
        src/source/render/square.h
        src/source/math/mathy.h
        src/source/render/texture.h
)

target_link_libraries(${PROJECT_NAME} 
        PRIVATE 
        SDL2::SDL2
        SDL2_image
)

target_include_directories(${PROJECT_NAME} 
        PRIVATE 
        ${sdl_BINARY_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        glm
)