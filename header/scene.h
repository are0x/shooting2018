#pragma once

#include <vector>
#include <memory>
#include "enemy.h"
#include "time_manager.h"

class Scene {
public:
  virtual std::vector<std::unique_ptr<Enemy>> Update();
};

///////////////////////////////////////////////////////////////////////////////

class SimpleScene : public Scene {
  double startTime;
  std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>> schedule;
  int doneCount;
public:
  SimpleScene(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule);
  void Start();
  std::vector<std::unique_ptr<Enemy>> Update();
};
