#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <GL/glut.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <complex>
#include <string>
#include <memory>
#include <unordered_set>

using namespace std;

constexpr int FPS = 60;

///////////////////////////////////////////////////////////////////////////////

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
  
  bool IsOverlapC(const Circle &x) {
    return abs(c - x.Center()) <= r + x.Radius();
  }
};

const int Circle::ACCURACY = 180;

///////////////////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////////////////

class PlayerBullet{
  Circle circle;
  Point2d velocity;
  int hp;
public:
  PlayerBullet(const Circle& circle, Point2d velocity) {
    this->circle = circle;
    this->velocity = velocity;
    this->hp = 1;
  }
  void Update(){
    Point2d nCenter = velocity / (double)FPS + circle.Center();
    this->circle = Circle{nCenter.real(), nCenter.imag(), circle.Radius()};
  }
  void Draw() {
    this->circle.Draw();
  }
  Circle HitBody() {
    return this->circle;
  }
  bool IsDead() {
    return hp <= 0;
  }
  void AddDamage(int damage) {
    hp -= damage;
  }
};

///////////////////////////////////////////////////////////////////////////////

class Player {
  static const double SPEED;
  Circle circle;
  int lastFireTime;
public:
  Player(const Circle& circle) {
    this->circle = circle;
    this->lastFireTime = 0;
  }
  
  vector<unique_ptr<PlayerBullet>> Update() {
    // move
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
      dvector = SPEED / FPS * (dvector / dvectorLength);

      Point2d nCenter = circle.Center() + dvector;
      this->circle = Circle{nCenter.real(), nCenter.imag(), circle.Radius()};
    }

    // shoot
    vector<unique_ptr<PlayerBullet>> bullets;
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    if(keyboard->IsHeld('n') && currentTime - lastFireTime > 200) {
      lastFireTime = currentTime;
      // TODO: pass parameters
      bullets.emplace_back(new PlayerBullet(this->circle, Point2d(0.0, 1.0)));
    }
    return move(bullets);
  }
  void Draw() {
    this->circle.Draw();
  }
  Circle HitBody() {
    return this->circle;
  }
};

const double Player::SPEED = 0.5;

///////////////////////////////////////////////////////////////////////////////

class EnemyBullet{
public:
  void Update(){
  }
  void Draw() {
  }
};

///////////////////////////////////////////////////////////////////////////////

class Enemy{
  int hp;
  Circle circle;
  bool deadFlag;
public:
  Enemy(int hp, Circle circle) {
    this->hp = hp;
    this->circle = circle;
    this->deadFlag = false;
  }
  virtual void Update() = 0;
  virtual void Draw() {
    circle.Draw();
  }
  virtual bool IsDead() {
    return  hp <= 0;
 }
  Circle HitBody() {
    return this->circle;
  }
  void AddDamage(int damage) {
    this->hp -= damage;
  }
};

///////////////////////////////////////////////////////////////////////////////

class SimpleEnemy : public Enemy {
public:
  SimpleEnemy(Point2d pos):Enemy(5, Circle{pos.real(), pos.imag(), 0.1}) {}
  void Update() {}
};

///////////////////////////////////////////////////////////////////////////////

class Scene {
public:
  virtual vector<unique_ptr<Enemy>> Update() {
    return vector<unique_ptr<Enemy>>();
  }
};

///////////////////////////////////////////////////////////////////////////////

class EnemyFactory {
public:
  virtual unique_ptr<Enemy> make() = 0;
};

class SimpleEnemyFactory : public EnemyFactory {
  Point2d pos;
public:
  SimpleEnemyFactory(Point2d pos) {
    this->pos = pos;
  }
  unique_ptr< Enemy > make() {
    return unique_ptr< Enemy >(new SimpleEnemy(this->pos));
  }
};

shared_ptr<EnemyFactory> simpleEnemyFactory(Point2d pos) {
  return shared_ptr<EnemyFactory>(new SimpleEnemyFactory(pos));
}

class SimpleScene : public Scene {
  int startFrame;
  vector<pair<double, shared_ptr<EnemyFactory>>> schedule;
  int doneCount;
public:
  SimpleScene(const vector<pair<double, shared_ptr<EnemyFactory>>>& schedule) {
    this->schedule = schedule;
    this->doneCount = 0;
  }
  void Start() {
    startFrame = glutGet(GLUT_ELAPSED_TIME);
  }
  vector<unique_ptr<Enemy>> Update() {
    vector<unique_ptr<Enemy>> result;
    int currentFrame = glutGet(GLUT_ELAPSED_TIME);
    while( doneCount < (int)schedule.size() &&
	   schedule[doneCount].first < (currentFrame - startFrame) / 1000 ) {
      result.push_back(schedule[doneCount].second->make());
      doneCount++;
    }
    return result;
  }
};

///////////////////////////////////////////////////////////////////////////////

class Stage {
  unique_ptr<Player> player;
  vector<unique_ptr<Enemy>> enemies;
  vector<unique_ptr<PlayerBullet>> playerBullets;
  vector<unique_ptr<EnemyBullet>> enemyBullets;
  unique_ptr<Scene> scene;

public:
  Stage (const Player& player, unique_ptr<Scene> scene) {
    this->player = unique_ptr<Player>(new Player(player));
    this->scene = move(scene);
  }
  void Update() {
    // TODO: しんだおぶじぇくとをかいしゅう
    vector<unique_ptr<PlayerBullet>> newPlayerBullets = player->Update();
    
    for (auto& enemy : enemies) {
      enemy->Update();
    }
    for (auto& bullet : playerBullets) {
      bullet->Update();
    }
    for (auto& bullet : enemyBullets) {
      bullet->Update();
    }
    vector<unique_ptr<Enemy>> newEnemies = scene->Update();
    for(auto& newPlayerBullet : newPlayerBullets) {
      playerBullets.push_back(move(newPlayerBullet));
    }
     for(auto& newEnemy : newEnemies) {
      enemies.push_back(move(newEnemy));
    }
     
     //hit judge
     for (auto& bullet : playerBullets) {
       bool hitBulletFlag = false;
       for (auto& enemy : enemies) {
	 if (bullet->HitBody().IsOverlapC(enemy->HitBody())) {
	   enemy->AddDamage(1);
	   hitBulletFlag = true;
	 }
       }
       if (hitBulletFlag) {
	 bullet->AddDamage(1);
       }
     }

     //remove dead object
     auto enemiesNewEnd =
       remove_if(enemies.begin(), enemies.end(),
         [](unique_ptr<Enemy>& x)->bool{return x->IsDead();});
     enemies.erase(enemiesNewEnd, enemies.end());
     auto playerBulletNewEnd =
       remove_if (playerBullets.begin(), playerBullets.end(),
         [](unique_ptr<PlayerBullet>& x)->bool{return x->IsDead();});
     playerBullets.erase(playerBulletNewEnd, playerBullets.end());
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

///////////////////////////////////////////////////////////////////////////////

void keyboardDown(unsigned char key, int x, int y);
void keyboardUp(unsigned char key, int x, int y);
void display(void);
void idle(void);

///////////////////////////////////////////////////////////////////////////////

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
    lastTime = curTime;
    glutPostRedisplay();
  }
}
