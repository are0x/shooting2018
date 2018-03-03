#pragma once

#include "enemy.h"
#include "enemy_command.h"
#include "time_manager.h"
#include "player.h"
#include <vector>
#include <memory>

class SimpleEnemy : public Enemy {
  static const double DEGTORAD;
  
  double startTime;
  double commandTime;
  double existenceTime;
  std::complex<double> moveDir;
  std::vector<EnemyCommand> commands;
  int doneCount;
  
public:
  
  SimpleEnemy(
	      int hp,
	      Circle circle,
	      double startTime,
	      double existenceTime,
	      const std::vector<EnemyCommand>& commands);

  void Update();
};


class SimpleEnemyFactory : public EnemyFactory {
  int hp;
  Circle circle;
  double existenceTime;
  std::vector<EnemyCommand> commands;
public: 
  SimpleEnemyFactory(int hp, Circle circle, double existenceTime,
		     const std::vector<EnemyCommand>& commands);
  std::unique_ptr<Enemy> Make();
};

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands
);
