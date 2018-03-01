#include "simple_enemy.h"
#include <iostream>

SimpleEnemy::SimpleEnemy(
	      int hp,
	      Circle circle,
	      double startTime,
	      double existenceTime,
	      const std::vector<EnemyCommand>& commands) : Enemy(hp, circle){
  this->startTime = startTime;
  this->existenceTime = existenceTime;
  this->commandTime = 0;
  this->commands = commands;
  doneCount = 0;
}

void SimpleEnemy::Update() {
  double deltaTime = timeManager.GetDiff();
  while(doneCount < (int)commands.size()) {
    EnemyCommand& curCommand = commands[doneCount];
    if (curCommand.command == EnemyCommandType::move) {
      double t = std::stod(curCommand.args[0]);
      double vx = std::stod(curCommand.args[1]);
      double vy = std::stod(curCommand.args[2]);
      double exCommandTime = deltaTime;
      bool bNextCommand = commandTime + deltaTime > t;
      if (bNextCommand) {
	exCommandTime = t - commandTime;
	commandTime = 0;
	deltaTime -= exCommandTime; 
	doneCount++;
      }
      Point2d npos = circle.Center() +  exCommandTime * Point2d(vx, vy);
      circle = Circle{npos.real(), npos.imag(), circle.Radius()};
      if (!bNextCommand) {
	break;
      }
    } else if (curCommand.command == EnemyCommandType::stop) {
      double t = std::stod(curCommand.args[0]);
      if (commandTime + deltaTime > t) {
	commandTime = 0;
	deltaTime -= (t - commandTime); 
	doneCount++;
      } else {
	break;
      }
    } else {
      doneCount++;
    }
    //TODO pmove
    //TODO attack
  }
  commandTime += deltaTime;
}

///////////////////////////////////////////////////////////////////////////////

SimpleEnemyFactory::SimpleEnemyFactory(int hp, Circle circle, double existenceTime,
				       const std::vector<EnemyCommand>& commands) {
  this->hp = hp;
  this->circle = circle;
  this->existenceTime = existenceTime;
  this->commands = commands;
}
std::unique_ptr< Enemy > SimpleEnemyFactory::Make() {
  return std::unique_ptr< Enemy >(new SimpleEnemy(hp, circle, timeManager.GetNow(), existenceTime, commands));
}

///////////////////////////////////////////////////////////////////////////////

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands) {
  return std::shared_ptr<EnemyFactory>(new SimpleEnemyFactory(hp, circle, existenceTime, commands));
}
