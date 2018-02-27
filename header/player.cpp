#include "player.h"
#include "config.h"

PlayerBullet::PlayerBullet(const Circle& circle, Point2d velocity) {
  this->circle = circle;
  this->velocity = velocity;
  this->hp = 1;
}
void PlayerBullet::Update(){
  Point2d nCenter = velocity / (double)FPS + circle.Center();
  this->circle = Circle{nCenter.real(), nCenter.imag(), circle.Radius()};
}
void PlayerBullet::Draw() {
  this->circle.Draw();
}
Circle PlayerBullet::HitBody() {
  return this->circle;
}
bool PlayerBullet::IsDead() {
  return hp <= 0;
}
void PlayerBullet::AddDamage(int damage) {
  hp -= damage;
}

///////////////////////////////////////////////////////////////////////////////

Player::Player(const Circle& circle) {
  this->circle = circle;
  this->lastFireTime = 0;
  this->hp = 1;
}
Player:: Player(const Player& player) {
  this->circle = player.HitBody();
  this->lastFireTime = 0;
  this->hp = 1;
}
  
std::vector<std::unique_ptr<PlayerBullet>> Player::Update() {
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
  std::vector<std::unique_ptr<PlayerBullet>> bullets;
  //TODO get currentTime 
  //int currentTime = glutGet(GLUT_ELAPSED_TIME);
  int currentTime = 0;
  if(keyboard->IsHeld('n') && currentTime - lastFireTime > 200) {
    lastFireTime = currentTime;
    // TODO: pass parameters
    bullets.emplace_back(new PlayerBullet(this->circle, Point2d(0.0, 1.0)));
  }
  return move(bullets);
}
void Player::Draw() {
  this->circle.Draw();
}
void Player::MoveOutOfDisplay() {
  this->circle = Circle{10000,10000, circle.Radius()};
}
Circle Player::HitBody() const {
  return this->circle;
}
void Player::AddDamage(int damage) {
  hp -= damage;
}
bool Player::IsDead() {
  return hp <= 0;
}

const double Player::SPEED = 0.5;
