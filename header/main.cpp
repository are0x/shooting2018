#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <GLFW/glfw3.h>
#include <string>
#include <memory>
#include <unordered_set>
#include "circle.h"
#include "config.h"
#include "enemy.h"
#include "keyboard.h"
#include "player.h"
#include "scene.h"
#include "simple_enemy.h"
#include "stage.h"
#include "time_manager.h"

using namespace std;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void display(void);
//void idle(void);

///////////////////////////////////////////////////////////////////////////////

unique_ptr<Keyboard> keyboard;
unique_ptr<Stage> stage;
are_time::TimeManager timeManager;

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
  /* Initialize the library */
  if (!glfwInit()) {
        return -1;
  }

  /* Create a windowed mode window and its OpenGL context */
  unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>
    window(glfwCreateWindow(640, 480, "シューティンギュ", NULL, NULL), glfwDestroyWindow);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window.get());

  //set key call back
  glfwSetKeyCallback(window.get(), key_callback);

  //view port setting
  glViewport(0, 0, 640, 480);
  glOrtho(0, 640, 0, 480, -1, 1);

  //init stage
  Player player(Circle{320, 100, 20});
  unique_ptr<Scene> scene(new SimpleScene(vector<pair<double, shared_ptr<EnemyFactory>>>{
    {0.0, simpleEnemyFactory(Point2d(100, 300))},
    {3.0, simpleEnemyFactory(Point2d(200, 300))},
    {6.0, simpleEnemyFactory(Point2d(300, 300))},
    {9.0, simpleEnemyFactory(Point2d(400, 300))}
  }));
  stage = unique_ptr<Stage>(new Stage(player, move(scene)));
  assert(!scene);
  
  keyboard = unique_ptr<Keyboard>(new Keyboard());
  
  stage->Update();

  glfwSetTime(0);
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window.get())) {
    float nowTime = glfwGetTime();
    if (nowTime - timeManager.GetNow() <= 1.0 / FPS) {
      continue;
    }
    timeManager.Update(nowTime);
    keyboard->Update();
    /* Render here */
    display();
    /* Swap front and back buffers */
    glfwSwapBuffers(window.get());
    
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}

///////////////////////////////////////////////////////////////////////////////

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_PRESS) {
    keyboard->Press(key);
  } else if (action == GLFW_RELEASE) {
    keyboard->Release(key);
  }
  
}

void display()
{
  stage->Update();
  
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 0.0f, 0.0f);
  stage->Draw();
}
