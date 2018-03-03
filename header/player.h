#pragma once

#include "circle.h"
#include "keyboard.h"
#include "time_manager.h"
#include <vector>
#include <memory>

class PlayerBullet{
  Circle circle;
  Point2d velocity;
  int hp;
public:
  PlayerBullet(const Circle& circle, Point2d velocity);
  void Update();
  void Draw();
  Circle HitBody();
  bool IsDead();
  void AddDamage(int damage);
};

///////////////////////////////////////////////////////////////////////////////

class Player {
  static const double SPEED;
  Circle circle;
  double lastFireTime;
  int hp;
public:
  Player(const Circle& circle);
  Player(const Player& player);
  
  std::vector<std::unique_ptr<PlayerBullet>> Update();
  virtual void Draw();
  void MoveOutOfDisplay();
  Circle HitBody() const;
  void AddDamage(int damage);
  bool IsDead();
};

extern std::unique_ptr<Player> gPlayer;
