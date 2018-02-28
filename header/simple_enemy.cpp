#include "simple_enemy.h"

SimpleEnemy::SimpleEnemy(Point2d pos):Enemy(5, Circle{pos.real(), pos.imag(), 10}) {}
void SimpleEnemy::Update() {}

///////////////////////////////////////////////////////////////////////////////

SimpleEnemyFactory::SimpleEnemyFactory(Point2d pos) {
  this->pos = pos;
}
std::unique_ptr< Enemy > SimpleEnemyFactory::Make() {
  return std::unique_ptr< Enemy >(new SimpleEnemy(this->pos));
}

///////////////////////////////////////////////////////////////////////////////

std::shared_ptr<EnemyFactory> simpleEnemyFactory(Point2d pos) {
  return std::shared_ptr<EnemyFactory>(new SimpleEnemyFactory(pos));
}
