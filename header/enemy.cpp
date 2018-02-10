#include "enemy.h"

void EnemyBullet::Update(){
  // TODO: implement this
}
void EnemyBullet::Draw() {
  // TODO: implement this
}
///////////////////////////////////////////////////////////////////////////////

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
