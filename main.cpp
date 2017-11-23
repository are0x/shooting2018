#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <complex>
#include <string>
#include <memory>
#include <unordered_set>

using namespace std;

using Point2d = complex<double>;

class Circle {
private:
  static const int ACCURACY;
  Point2d c;
  double r;
  // TODO: have color as a field

public:
  Circle(){}
  Circle(double x, double y, double r) {
    this->c = Point2d{x, y};
    this->r = r;
  }
  Point2d Center() const {return c;}
  double Radius() const {return r;}
  void Draw() {
    // TODO: set color
    glBegin(GL_POLYGON);

    double x = Center().real();
    double y = Center().imag();
    double r = Radius();
    for (int i = 0;i < ACCURACY;i++) {
      double rad = 2 * M_PI / ACCURACY * i;
      glVertex2d(x + r * cos(rad), y + r * sin(rad));
    }
    glEnd();    
  }
};

const int Circle::ACCURACY = 180;

class Keyboard {
  unordered_set<unsigned char> pressed;
  unordered_set<unsigned char> held;

public:
  bool IsPressed(unsigned char key) {
    return pressed.find(key) != pressed.end();
  }
  bool IsHeld(unsigned char key){
    return IsPressed(key) || held.find(key) != held.end();
  }
  void Update(){
    pressed.clear();
  }
  void Press(unsigned char key) {
    pressed.insert(key);
    held.insert(key);
  }
  void Release(unsigned char key) {
    held.erase(key);
  }
};

unique_ptr<Keyboard> keyboard;

class Player {
  static const double SPEED;
  Circle circle;
  
public:
  Player(const Circle& circle) {
    this->circle = circle;
  }
  
  void Update() {
    double dx = 0.0, dy = 0.0;
    if(keyboard->IsHeld('w')) {
      dy += 1;
    }
    if(keyboard->IsHeld('s')) {
      dy -= 1;
    }
    if(keyboard->IsHeld('a')) {
      dx -= 1;
    }
    if(keyboard->IsHeld('d')) {
      dx += 1;
    }
    Point2d dvector(dx, dy);
    double dvectorLength = abs(dvector);
    if (dvectorLength > 0) {
      dvector = SPEED * (dvector / dvectorLength);

      Point2d nCenter = circle.Center() + dvector;
      this->circle = Circle{nCenter.real(), nCenter.imag(), circle.Radius()};
    }
  }
  void Draw() {
    this->circle.Draw();
  }
};

const double Player::SPEED = 0.008;

class Enemy{
public:
  void Update(){
  }
  void Draw() {
  }
};

class PlayerBullet{
public:
  void Update(){
  }
  void Draw() {
  }
};

class EnemyBullet{
public:
  void Update(){
  }
  void Draw() {
  }
};

class Stage {
  unique_ptr<Player> player;
  vector<unique_ptr<Enemy>> enemies;
  vector<unique_ptr<PlayerBullet>> playerBullets;
  vector<unique_ptr<EnemyBullet>> enemyBullets;

public:
  Stage (const Player& player) {
    this->player = unique_ptr<Player>(new Player(player));
  }
  void Update() {
    player->Update();
    for (auto& enemy : enemies) {
      enemy->Update();
    }
    for (auto& bullet : playerBullets) {
      bullet->Update();
    }
    for (auto& bullet : enemyBullets) {
      bullet->Update();
    }
    //TODO : あたりはんてい
  }

  void Draw() {
    player->Draw();
    for (auto& enemy : enemies) {
      enemy->Draw();
    }
    for (auto& bullet : playerBullets) {
      bullet->Draw();
    }
    for (auto& bullet : enemyBullets) {
      bullet->Draw();
    }
  }
};



void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void display(void);
void idle(void);

unique_ptr<Stage> stage;

int main(int argc, char** argv)
{
  //init stage
  Player player(Circle{0.0, 0.0, 0.03});
  stage = unique_ptr<Stage>(new Stage(player));

  keyboard = unique_ptr<Keyboard>(new Keyboard());
  
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
  if(curTime - lastTime > 1.0 / 60) {
    lastTime = curTime;
    glutPostRedisplay();
  }
}
