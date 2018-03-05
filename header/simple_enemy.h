#pragma once

#include "enemy.h"
#include "enemy_command.h"
#include "time_manager.h"
#include "player.h"
#include <vector>
#include <memory>

class SimpleEnemyFactory : public EnemyFactory {
  int hp;
  Circle circle;
  double existenceTime;
  std::vector<EnemyCommand> commands;
  std::vector<SimpleEnemyFactory> bulletFactory;
public: 
  SimpleEnemyFactory(int hp, Circle circle, double existenceTime,
		     const std::vector<EnemyCommand>& commands,
		     const std::vector<SimpleEnemyFactory>& bulletFactory);
  std::unique_ptr<Enemy> Make();
  void SetMakePos(Point2d p);
};

SimpleEnemyFactory simpleBulletFactory(double r, const std::vector<EnemyCommand>& commands);

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands,
						 const std::vector<SimpleEnemyFactory>& bulletFactory
);

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands
);

////////////////////////////////////////////////////////////////////////////////

class SimpleEnemy : public Enemy {
  static const double DEGTORAD;

  std::vector<std::unique_ptr<Enemy>> bullets;
  double startTime;
  double commandTime;
  double existenceTime;
  std::complex<double> moveDir;
  std::vector<EnemyCommand> commands;
  std::vector<SimpleEnemyFactory> bulletFactory;
  int doneCount;
  
public:
  SimpleEnemy(
	      int hp,
	      Circle circle,
	      double startTime,
	      double existenceTime,
	      const std::vector<EnemyCommand>& commands,
	      const std::vector<SimpleEnemyFactory>& bulletFactory);

  void Update();
  std::vector<std::unique_ptr<Enemy>> MakeBullet();
};  

////////////////////////////////////////////////////////////////////////////////

class SimpleEnemyBullet : public Enemy {
};
