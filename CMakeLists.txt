cmake_minimum_required(VERSION 3.26)
project(yumesdl)

set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    include(FetchContent)
    FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL
        GIT_TAG release-2.30.6
    )
    FetchContent_MakeAvailable(SDL)

    FetchContent_Declare(
        SDL_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image
        GIT_TAG release-2.8.2
    )
    FetchContent_MakeAvailable(SDL_image)

    FetchContent_Declare(
        SDL_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
        GIT_TAG release-2.8.0
    )
    FetchContent_MakeAvailable(SDL_mixer)

    FetchContent_Declare(
        SDL_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf
        GIT_TAG release-2.22.0
    )
    FetchContent_MakeAvailable(SDL_ttf)

else()
# REQUIRE - CMAKE, GCC, SDL2, SDL_image, SDL_mixer, SDL_ttf
# sudo apt update
# sudo apt-get install build-essential cmake libasound2-dev libpulse-dev libudev-dev libdbus-1-dev libjack-dev libgl1-mesa-dev libx11-dev libxcursor-dev libxi-dev libxext-dev libxfixes-dev libxrandr-dev libxss-dev libxxf86vm-dev libwayland-dev libxkbcommon-dev libdrm-dev
# sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev libsdl2-ttf-dev
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED sdl2)
    pkg_check_modules(SDL2_image REQUIRED SDL2_image)
    pkg_check_modules(SDL2_mixer REQUIRED SDL2_mixer)
    pkg_check_modules(SDL2_ttf REQUIRED SDL2_ttf)
endif()

add_executable(${PROJECT_NAME}
    program/main.cpp
    program/config.hpp

    program/packages/math/math.hpp

    program/packages/game_objects/rocket.cpp
    program/packages/game_objects/rocket.hpp
    
    program/packages/ui_objects/text.cpp
    program/packages/ui_objects/text.hpp
 )

if (WIN32)
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
        SDL2_image
        SDL2_mixer
        SDL2_ttf
    )
else()
    target_link_libraries(${PROJECT_NAME}
        PRIVATE
        ${SDL2_LIBRARIES}
        ${SDL2_image_LIBRARIES}
        ${SDL2_mixer_LIBRARIES}
        ${SDL2_ttf_LIBRARIES}
    )
    target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_image_INCLUDE_DIRS}
        ${SDL2_mixer_INCLUDE_DIRS}
        ${SDL2_ttf_INCLUDE_DIRS}
    )
endif()
