# CMakeList.txt : CMake project for basic, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_executable (basic "basic.cpp" "basic.h" "solver.cpp" "solver.h" "renderer.h" "renderer.cpp" "model.h")

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
endif()

find_package(xtl REQUIRED)
find_package(xtensor REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
#set(SDL2_LIB c:/Libs/lib/SDL2/SDL2.lib)
#set(SDL2_MAIN_LIB c:/Libs/lib/SDL2/SDL2main.lib)



target_link_libraries(basic PUBLIC xtensor)
target_include_directories(basic PUBLIC ${xtensor_INCLUDE_DIRS})

#set(SDL_INCLUDE_DIRS c:/Libs/include/SDL2)
target_link_libraries(basic PUBLIC ${SDL2_LIBRARIES})
target_include_directories(basic PUBLIC ${SDL_INCLUDE_DIRS})
target_link_libraries(basic PUBLIC SDL2::SDL2main SDL2::SDL2)

target_link_libraries(basic PUBLIC ${OPENGL_LIBRARY})
target_include_directories(basic PUBLIC ${OPENGL_INCLUDE_DIRS})
message(STATUS "<<${OPENGL_INCLUDE_DIRS}>>")
# TODO: Add tests and install targets if needed.


