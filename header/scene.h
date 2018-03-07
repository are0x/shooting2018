#pragma once

#include <vector>
#include <memory>
#include "enemy.h"
#include "time_manager.h"

class Scene {
public:
  virtual std::vector<std::unique_ptr<Enemy>> Update();
  virtual bool IsEnd() = 0;
  virtual void Start() = 0;
};

class SceneFactory {
 public:
  virtual std::unique_ptr<Scene> Make() = 0;
};

///////////////////////////////////////////////////////////////////////////////

class SimpleScene : public Scene {
  double startTime;
  double duration;
  std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>> schedule;
  int doneCount;
public:
  SimpleScene(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule, double duration);
  void Start();
  std::vector<std::unique_ptr<Enemy>> Update();
  bool IsEnd();
};

///////////////////////////////////////////////////////////////////////////////

class SimpleSceneFactory : public SceneFactory {
  double duration;
  std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>> schedule;
 public:
  SimpleSceneFactory(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule, double duration);
  std::unique_ptr<Scene> Make();
};
