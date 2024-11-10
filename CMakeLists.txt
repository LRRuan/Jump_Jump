cmake_minimum_required(VERSION 3.26)
project(untitled C)

set(CMAKE_C_STANDARD 17)


include_directories("C:/c/mingw(CLion+VSC)/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/include")
include_directories("include")
link_directories("C:/c/mingw(CLion+VSC)/mingw(CLion+VSC)/SDL2-2.26.0-allinone/x86_64-w64-mingw32/lib")

add_executable(cproject src/main.c src/SDLcommon.c src/page.c src/game.c)

target_link_libraries(cproject PRIVATE
        mingw32
        SDL2main
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf
)
