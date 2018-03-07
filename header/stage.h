#pragma once
#include "player.h"
#include "enemy.h"
#include "scene.h"
#include "random_util.h"

class Stage {
  int respawnCounter;
  int clearSceneNum;
  int clearConditionNum;
  std::vector<std::unique_ptr<Enemy>> enemies;
  std::vector<std::unique_ptr<PlayerBullet>> playerBullets;
  std::vector<std::unique_ptr<Enemy>> enemyBullets;
  std::unique_ptr<Scene> nowScene;
  std::unique_ptr<Scene> nextScene;
  std::vector<std::shared_ptr<SceneFactory>> scenes;
  RandomUtil gen;

  std::unique_ptr<Scene> DrowScene();

public:
  Stage (const std::vector<std::shared_ptr<SceneFactory>>& scenes, int clearNum);
  void Start();
  void Update();
  void Draw();
};
