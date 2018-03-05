#include "simple_enemy.h"
#include <iostream>
#include <cmath>

const double SimpleEnemy::DEGTORAD = 2 * M_PI / 360;

SimpleEnemy::SimpleEnemy(
	      int hp,
	      Circle circle,
	      double startTime,
	      double existenceTime,
	      const std::vector<EnemyCommand>& commands,
	      const std::vector<SimpleEnemyFactory>& bulletFactory) : Enemy(hp, circle){
  this->startTime = startTime;
  this->existenceTime = existenceTime;
  this->commandTime = 0;
  this->commands = commands;
  doneCount = 0;
  this->moveDir = {1, 0};
  this->bulletFactory = bulletFactory;
}

void SimpleEnemy::Update() {
  bullets.clear();
  double deltaTime = timeManager.GetDiff();
  while(doneCount < (int)commands.size()) {
    EnemyCommand& curCommand = commands[doneCount];
    if (curCommand.command == EnemyCommandType::move) {
      double t = std::stod(curCommand.args[0]);
      double v = std::stod(curCommand.args[1]);
      double exCommandTime = deltaTime;
      bool bNextCommand = commandTime + deltaTime > t;
      if (bNextCommand) {
	exCommandTime = t - commandTime;
	commandTime = 0;
	deltaTime -= exCommandTime; 
	doneCount++;
      }
      Point2d npos = circle.Center() +  exCommandTime * v * moveDir;
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
    } else if (curCommand.command == EnemyCommandType::mangle) {
      double rad = std::stod(curCommand.args[0]) * DEGTORAD;
      moveDir = std::polar(1.0, rad);
    } else if (curCommand.command == EnemyCommandType::mpangle) {
      double rad = std::stod(curCommand.args[0]) * DEGTORAD;
      std::complex<double> rot = std::polar(1.0, rad);
      std::complex<double> epv = gPlayer->HitBody().Center() - circle.Center();
      std::complex<double> nepv = std::polar(1.0, std::arg(epv));
      moveDir = nepv * rot;
    } else if (curCommand.command == EnemyCommandType::bullet) {
      int bulNum = bulletFactory.size();
      int bulId = std::stoi(curCommand.args[0]);
      
      if (bulId >= bulNum) {
	continue;
      }
      double relX = std::stod(curCommand.args[1]);
      double relY = std::stod(curCommand.args[2]);
      auto factory = bulletFactory[bulId];
      factory.SetMakePos(Point2d(relX, relY) + circle.Center());
      bullets.emplace_back(factory.Make());
    }
    doneCount++;
  }
  commandTime += deltaTime;
}

std::vector<std::unique_ptr<Enemy>> SimpleEnemy::MakeBullet() {
  return move(bullets);
}

///////////////////////////////////////////////////////////////////////////////

SimpleEnemyFactory::SimpleEnemyFactory(int hp, Circle circle, double existenceTime,
				       const std::vector<EnemyCommand>& commands,
				       const std::vector<SimpleEnemyFactory>& bulletFactory) {
  this->hp = hp;
  this->circle = circle;
  this->existenceTime = existenceTime;
  this->commands = commands;
  this->bulletFactory = bulletFactory;
}

std::unique_ptr< Enemy > SimpleEnemyFactory::Make() {
  return std::unique_ptr< Enemy >(new SimpleEnemy(hp, circle, timeManager.GetNow(), existenceTime, commands, bulletFactory));
}

void SimpleEnemyFactory::SetMakePos(Point2d p) {
  this->circle = Circle(p.real(), p.imag(), this->circle.Radius());
}

///////////////////////////////////////////////////////////////////////////////

SimpleEnemyFactory simpleBulletFactory(double radius, const std::vector<EnemyCommand>& commands) {
  return SimpleEnemyFactory(1, Circle(0, 0, radius), 1000, commands, std::vector<SimpleEnemyFactory>());
}

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands,
						 const std::vector<SimpleEnemyFactory>& bulletFactory) {
  return std::shared_ptr<EnemyFactory>(new SimpleEnemyFactory(hp, circle, existenceTime, commands, bulletFactory));
}

std::shared_ptr<EnemyFactory> simpleEnemyFactory(int hp, Circle circle, double existenceTime,
						 const std::vector<EnemyCommand>& commands
						 ){
  return std::shared_ptr<EnemyFactory>(new SimpleEnemyFactory(hp, circle, existenceTime, commands, std::vector<SimpleEnemyFactory>()));
}
