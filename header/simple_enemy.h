#pragma once

#include "enemy.h"
#include <memory>

class SimpleEnemy : public Enemy {
public:
  SimpleEnemy(Point2d pos);
  void Update();
};


class SimpleEnemyFactory : public EnemyFactory {
  Point2d pos;
public:
  SimpleEnemyFactory(Point2d pos);
  std::unique_ptr<Enemy> Make();
};

std::shared_ptr<EnemyFactory> simpleEnemyFactory(Point2d pos);
