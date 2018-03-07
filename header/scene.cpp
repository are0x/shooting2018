#include "scene.h"

std::vector<std::unique_ptr<Enemy>> Scene::Update() {
  return std::vector<std::unique_ptr<Enemy>>();
}

SimpleScene::SimpleScene(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule, double duration) {
  this->schedule = schedule;
  this->doneCount = 0;
  this->duration = duration;
}

void SimpleScene::Start() {
  startTime = timeManager.GetNow();
}

std::vector<std::unique_ptr<Enemy>> SimpleScene::Update() {
  std::vector<std::unique_ptr<Enemy>> result;
  double nowTime = timeManager.GetNow();
  while( doneCount < (int)schedule.size() &&
	 schedule[doneCount].first < nowTime - startTime ) {
    result.push_back(schedule[doneCount].second->Make());
    doneCount++;
  }
  return result;
}

bool SimpleScene::IsEnd() {
  return (timeManager.GetNow() - startTime) >= duration;
}

///////////////////////////////////////////////////////////////////////////////

SimpleSceneFactory::SimpleSceneFactory(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule, double duration) {
  this->schedule = schedule;
  this->duration = duration;
}

std::unique_ptr<Scene> SimpleSceneFactory::Make() {
  return std::unique_ptr<Scene>(new SimpleScene(schedule, duration));
}
