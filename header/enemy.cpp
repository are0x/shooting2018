#include "enemy.h"

Enemy::Enemy(int hp, Circle circle) {
  this->hp = hp;
  this->circle = circle;
  this->deadFlag = false;
}
void Enemy::Draw() {
  circle.Draw();
}
bool Enemy::IsDead() {
  return  hp <= 0;
}
Circle Enemy::HitBody() {
  return this->circle;
}
void Enemy::AddDamage(int damage) {
  this->hp -= damage;
}

std::vector<std::unique_ptr<Enemy>> Enemy::MakeBullet() {
  return std::vector<std::unique_ptr<Enemy>>();
}
