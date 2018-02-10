#pragma once
#include "player.h"
#include "enemy.h"
#include "scene.h"

class Stage {
  std::unique_ptr<Player> initPlayer;
  int respawnCounter;
  std::unique_ptr<Player> player;
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<std::unique_ptr<PlayerBullet>> playerBullets;
  std::vector<std::unique_ptr<EnemyBullet>> enemyBullets;
  std::unique_ptr<Scene> scene;

public:
  Stage (const Player& player, std::unique_ptr<Scene> scene);
  void Update();
  void Draw();
};
