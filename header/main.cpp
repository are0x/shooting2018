#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <cmath>
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


void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void display(void);
void idle(void);

///////////////////////////////////////////////////////////////////////////////

unique_ptr<Keyboard> keyboard;
unique_ptr<Stage> stage;

///////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
  //init stage
  Player player(Circle{0.0, 0.0, 0.03});
  unique_ptr<Scene> scene(new SimpleScene(vector<pair<double, shared_ptr<EnemyFactory>>>{
    {0.0, simpleEnemyFactory(Point2d(0.0, 0.5))},
    {3.0, simpleEnemyFactory(Point2d(-0.3, 0.5))},
    {6.0, simpleEnemyFactory(Point2d(0.3, 0.5))},
    {9.0, simpleEnemyFactory(Point2d(0.0, 0.5))}
  }));
  stage = unique_ptr<Stage>(new Stage(player, move(scene)));
  assert(!scene);
  
  keyboard = unique_ptr<Keyboard>(new Keyboard());
  
  stage->Update();

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutInit(&argc, argv);
  glutCreateWindow("GLUT Test");
  glutKeyboardFunc(&keyboardDown);
  glutKeyboardUpFunc(&keyboardUp);
  glutIdleFunc(&idle);
  glutDisplayFunc(&display);

  glutMainLoop();

  return EXIT_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////

void keyboardDown(unsigned char key, int x, int y)
{
  printf( "press: %x (%c)\n", key, key );
  fflush(stdout);
  keyboard->Press(key);
  switch (key)
  {
    case '\x1B':
      exit(EXIT_SUCCESS);
      break;
  }
}

void keyboardUp(unsigned char key, int x, int y)
{
  printf( "release: %x (%c)\n", key, key );
  fflush(stdout);
  keyboard->Release(key);
}


void display()
{
  stage->Update();
  keyboard->Update();
  
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0f, 0.0f, 0.0f);
  stage->Draw();
  glutSwapBuffers();
}

void idle(void)
{
  static double lastTime = 0.0;
  double curTime =  glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
  if(curTime - lastTime > 1.0 / FPS) {
    time::diff = curTime - lastTime;
    lastTime = curTime;
    glutPostRedisplay();
  }
}
