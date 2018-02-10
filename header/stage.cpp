#include "stage.h"
#include <algorithm>

Stage::Stage (const Player& player, std::unique_ptr<Scene> scene) {
  this->initPlayer = std::unique_ptr<Player>(new Player(player));
  this->player = std::unique_ptr<Player>(new Player(player));
  this->scene = move(scene);
  this->respawnCounter = 0;
}

void Stage::Update() {
  std::vector<std::unique_ptr<PlayerBullet>> newPlayerBullets = player->Update();
    
  for (auto& enemy : enemies) {
    enemy->Update();
  }
  for (auto& bullet : playerBullets) {
    bullet->Update();
  }
  for (auto& bullet : enemyBullets) {
    bullet->Update();
  }
  std::vector<std::unique_ptr<Enemy>> newEnemies = scene->Update();
  for(auto& newPlayerBullet : newPlayerBullets) {
    playerBullets.push_back(move(newPlayerBullet));
  }
  for(auto& newEnemy : newEnemies) {

    enemies.push_back(move(newEnemy));
  }
     
  //hit judge
  for (auto& bullet : playerBullets) {
    bool hitBulletFlag = false;
    for (auto& enemy : enemies) {
      if (bullet->HitBody().IsOverlapC(enemy->HitBody())) {
	enemy->AddDamage(1);
	hitBulletFlag = true;
      }
    }
    if (hitBulletFlag) {
      bullet->AddDamage(1);
    }
  }

  for (auto& enemy : enemies) {
    if (enemy->HitBody().IsOverlapC(player->HitBody())) {
      enemy->AddDamage(1000000);
      player->AddDamage(1);
    }
  }

  //TODO : remove out of range bullet 
  //remove dead object
  auto enemiesNewEnd =
    std::remove_if(enemies.begin(), enemies.end(),
	      [](std::unique_ptr<Enemy>& x)->bool{return x->IsDead();});
  enemies.erase(enemiesNewEnd, enemies.end());
  auto playerBulletNewEnd =
    std::remove_if (playerBullets.begin(), playerBullets.end(),
	       [](std::unique_ptr<PlayerBullet>& x)->bool{return x->IsDead();});
  playerBullets.erase(playerBulletNewEnd, playerBullets.end());
  if (player->IsDead()) {
    player->MoveOutOfDisplay();
    if (keyboard->IsPressed('r') && respawnCounter < 3) {
      player = std::unique_ptr<Player>(new Player(*initPlayer));
      respawnCounter++;
    }
  }
}

void Stage::Draw() {
  player->Draw();
    
  for (auto& enemy : enemies) {
    enemy->Draw();
  }
  for (auto& bullet : playerBullets) {
    bullet->Draw();
  }
  for (auto& bullet : enemyBullets) {
    bullet->Draw();
  }
}
