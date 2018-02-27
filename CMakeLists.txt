cmake_minimum_required(VERSION 2.8)

project(ShootingMock CXX)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

# gcc -Wall オプションを指定する
set(CMAKE_CXX_FLAGS "-g -Wall")

add_executable(Main
  header/main.cpp
  header/circle.cpp
  header/enemy.cpp
  header/keyboard.cpp
  header/player.cpp
  header/scene.cpp
  header/simple_enemy.cpp
  header/stage.cpp
  header/time_manager.cpp
)

IF(APPLE)
    find_library( OpenGL_LIBRARY OpenGL )
    SET( EXTRA_LIBS ${OpenGL_LIBRARY})
ELSE()
    SET( EXTRA_LIBS "opengl32")
ENDIF()

target_link_libraries(Main glfw3 ${EXTRA_LIBS})

