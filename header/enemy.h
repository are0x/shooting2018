#pragma once

#include "circle.h"
#include "enemy_command.h"
#include <memory>

class EnemyBullet {
public:
  void Update();
  void Draw();
};

///////////////////////////////////////////////////////////////////////////////

class Enemy{
  int hp;
  bool deadFlag;
 protected:
  Circle circle;
 public:
  Enemy(int hp, Circle circle);
  virtual void Update() = 0;
  virtual void Draw();
  virtual bool IsDead();
  Circle HitBody();
  void AddDamage(int damage);
};

class EnemyFactory {
public:
  virtual std::unique_ptr<Enemy> Make() = 0;
};
