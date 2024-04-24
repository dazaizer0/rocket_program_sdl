cmake_minimum_required(VERSION 3.26)
project(yumesdl)

set(CMAKE_CXX_STANDARD 20)
set(SDL_SHARED ON)

if(WIN32)

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

    FetchContent_Declare(
            SDL_mixer
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer
            GIT_TAG release-2.8.0
    )
    FetchContent_MakeAvailable(SDL_mixer)

elseif(UNIX AND NOT APPLE)

    find_package(SDL2 REQUIRED)

endif()


add_executable(${PROJECT_NAME}
        src/main.cpp
        src/config.h

        src/mathy.hpp
)


if(WIN32)

    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            SDL2::SDL2
            SDL2_image
            SDL2_mixer
    )

    target_include_directories(${PROJECT_NAME}
            PRIVATE
            ${SDL2_INCLUDE_DIRS}
    )

endif()