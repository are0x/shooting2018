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
unique_ptr<Player> gPlayer(new Player(Circle{320, 100, 20}));
are_time::TimeManager timeManager;
std::vector<std::vector<EnemyCommand>> enemyCommands{
  {
    makeEnemyCommand("mangle 30"),
    makeEnemyCommand("move 10 160"),
  },
  {
    makeEnemyCommand("bullet 0"),
      makeEnemyCommand("mangle 270"),
      makeEnemyCommand("move 0.5 160"),
      makeEnemyCommand("bullet 0"),
      makeEnemyCommand("move 1 160"),
      makeEnemyCommand("bullet 0"),
  },
  {
    makeEnemyCommand("mpangle 0"),
    makeEnemyCommand("move 10 150"),
  },
    {
      makeEnemyCommand("mangle 270"),
      makeEnemyCommand("move 20 400")
    },
    {
      makeEnemyCommand("mangle 270"),
      makeEnemyCommand("move 20 200")
    },
    {
      makeEnemyCommand("mangle 0"),
      makeEnemyCommand("move 0.2 800"),
      makeEnemyCommand("mangle 270"),
      makeEnemyCommand("move 0.3 600"),
      makeEnemyCommand("mangle 0"),
      makeEnemyCommand("move 20 800"),
    },
    {
      makeEnemyCommand("mangle 90"),
      makeEnemyCommand("move 0.8 800"),
      makeEnemyCommand("mangle 0"),
      makeEnemyCommand("move 0.5 800"),
      makeEnemyCommand("mangle -90"),
      makeEnemyCommand("move 2 800"),
    },
    {
      makeEnemyCommand("mangle 90"),
      makeEnemyCommand("move 0.8 800"),
      makeEnemyCommand("mangle 180"),
      makeEnemyCommand("move 0.5 800"),
      makeEnemyCommand("mangle -90"),
      makeEnemyCommand("move 2 800"),
    },
};

std::vector<SimpleEnemyFactory> bulletFactorys{
  simpleBulletFactory(10, enemyCommands[2]),
};

SimpleSceneFactory StraightScene(int startx, int dx, int cmdIdx) {
  vector<pair<double, shared_ptr<EnemyFactory>>> schedule;
  int num = 20;
  for (int i = 0;i < num;i++) {
    schedule.push_back({0.3 + i * 0.2 ,simpleEnemyFactory(1, Circle(startx + (i % 2 * dx), 800, 30), 20, enemyCommands[cmdIdx])});
  }
  return SimpleSceneFactory(schedule, 4.8);
}

SimpleSceneFactory RepEnemyScene(int startx, int starty, int num, double dsec, double sec, const std::vector<EnemyCommand>& cmds) {
  vector<pair<double, shared_ptr<EnemyFactory>>> schedule;
  for (int i = 0;i < num;i++) {
    schedule.push_back({dsec + i * 0.2 ,simpleEnemyFactory(1, Circle(startx, starty, 30), 20, cmds)});
  }
  return SimpleSceneFactory(schedule, sec);
}

std::vector<SimpleSceneFactory> sceneFactorys{
  SimpleSceneFactory(vector<pair<double, shared_ptr<EnemyFactory>>>{
	{0.0, simpleEnemyFactory(1, Circle(100, 300, 40), 10, enemyCommands[0])},
	{3.0, simpleEnemyFactory(1, Circle(200, 300, 40), 10, enemyCommands[1], bulletFactorys)},
	{6.0, simpleEnemyFactory(1, Circle(300, 300, 40), 10, enemyCommands[0])},
	{9.0, simpleEnemyFactory(1, Circle(400, 300, 40), 10, enemyCommands[1], bulletFactorys)},
      },
    10.0),
   SimpleSceneFactory(vector<pair<double, shared_ptr<EnemyFactory>>>{
	{0.0, simpleEnemyFactory(1, Circle(100, 300, 20), 10, enemyCommands[0])},
	{3.0, simpleEnemyFactory(1, Circle(200, 300, 20), 10, enemyCommands[1], bulletFactorys)},
	{6.0, simpleEnemyFactory(1, Circle(300, 300, 20), 10, enemyCommands[0])},
	{9.0, simpleEnemyFactory(1, Circle(400, 300, 20), 10, enemyCommands[1], bulletFactorys)},
      },
    10.0),
    StraightScene(100, 100, 2),
    StraightScene(500, -100, 2),
    StraightScene(100, 100, 3),
    StraightScene(500, -100, 3),
    RepEnemyScene(0, 700, 14, 0.3, 4.8, enemyCommands[5]),
    RepEnemyScene(100, 0, 14, 0.3, 4.8, enemyCommands[6]),
    RepEnemyScene(500, 0, 14, 0.3, 4.8, enemyCommands[7]),
    };

///////////////////////////////////////////////////////////////////////////////

int main(void)
{
  /* Initialize the library */
  if (!glfwInit()) {
        return -1;
  }

  int w = 600;
  int h = 800;

  /* Create a windowed mode window and its OpenGL context */
  unique_ptr<GLFWwindow, decltype(&glfwDestroyWindow)>
    window(glfwCreateWindow(w, h, "シューティンギュ", NULL, NULL), glfwDestroyWindow);
  if (!window) {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window.get());

   //set key call back
  glfwSetKeyCallback(window.get(), key_callback);

  //view port setting
  
  glViewport(0, 0, w, h);
  glOrtho(0, w, 0, h, -1, 1);

  //init stage
  std::vector<std::shared_ptr<SceneFactory>> stageSceneFactories = {
    std::shared_ptr<SceneFactory>(&sceneFactorys[2]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[3]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[4]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[5]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[6]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[7]),
    std::shared_ptr<SceneFactory>(&sceneFactorys[8]),
  };
  stage = unique_ptr<Stage>(new Stage(stageSceneFactories, 7));
  stage->Start();
  
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
