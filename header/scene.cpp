#include "scene.h"

std::vector<std::unique_ptr<Enemy>> Scene::Update() {
  return std::vector<std::unique_ptr<Enemy>>();
}

SimpleScene::SimpleScene(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule) {
  this->schedule = schedule;
  this->doneCount = 0;
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
