#include "scene.h"

std::vector<std::unique_ptr<Enemy>> Scene::Update() {
  return std::vector<std::unique_ptr<Enemy>>();
}



SimpleScene::SimpleScene(const std::vector<std::pair<double, std::shared_ptr<EnemyFactory>>>& schedule) {
  this->schedule = schedule;
  this->doneCount = 0;
}

void SimpleScene::Start() {
  //TODO get start frame
  //startFrame = glutGet(GLUT_ELAPSED_TIME);
  startFrame = 0;
}

std::vector<std::unique_ptr<Enemy>> SimpleScene::Update() {
  std::vector<std::unique_ptr<Enemy>> result;
  //TODO get current time
  //int currentFrame = glutGet(GLUT_ELAPSED_TIME);
  int currentFrame = 0;
  while( doneCount < (int)schedule.size() &&
	 schedule[doneCount].first < (currentFrame - startFrame) / 1000 ) {
    result.push_back(schedule[doneCount].second->Make());
    doneCount++;
  }
  return result;
}
