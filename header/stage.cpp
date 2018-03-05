#include "stage.h"
#include <algorithm>

Stage::Stage (std::unique_ptr<Scene> scene) {
  //this->initPlayer = gPlayer;
  //this->player = gPlayer;
  this->scene = move(scene);
  this->respawnCounter = 0;
}

void Stage::Update() {
  std::vector<std::unique_ptr<PlayerBullet>> newPlayerBullets = gPlayer->Update();
    
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

  //add bullet
  for (auto& enemy : enemies) {
    for(auto& newEnemyBullet : enemy->MakeBullet()) {
      enemyBullets.push_back(move(newEnemyBullet));
    }
  }

  //pop new enemy
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

  for (auto& bullet : enemyBullets) {
    if (bullet->HitBody().IsOverlapC(gPlayer->HitBody())) {
      gPlayer->AddDamage(1);
      bullet->AddDamage(1);
    }
  }

  for (auto& enemy : enemies) {
    if (enemy->HitBody().IsOverlapC(gPlayer->HitBody())) {
      enemy->AddDamage(1000000);
      gPlayer->AddDamage(1);
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
  if (gPlayer->IsDead()) {
    gPlayer->MoveOutOfDisplay();
    if (keyboard->IsPressed('r') && respawnCounter < 3) {
      //player = std::unique_ptr<Player>(new Player(*initPlayer));
      respawnCounter++;
    }
  }
}

void Stage::Draw() {
  gPlayer->Draw();
    
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
