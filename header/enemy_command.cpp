#include "enemy_command.h"
#include <sstream>
#include <iostream>

EnemyCommand::EnemyCommand(EnemyCommandType command, const std::vector<std::string>& args) {
  this->command = command;
  this->args = args;
}

EnemyCommandType makeEnemyCommandType(std::string str) {
  if (str == "move") {
    return EnemyCommandType::move;
  } else if (str == "pmove") {
    return EnemyCommandType::pmove;
  } else if (str == "stop") {
     return EnemyCommandType::stop;
  } else if (str == "attack") {
    return EnemyCommandType::attack;
  }
  return EnemyCommandType::nop;
}

EnemyCommand makeEnemyCommand(std::string raw) {
  std::vector<std::string> splited;
  std::stringstream ss;
  std::string item;
  std::string command;
  int cnt = 0;
  ss << raw;
  while (getline(ss, item, ' ') && !item.empty()) {
    if (cnt == 0) {
      command = item;
    } else {
      splited.push_back(item);
    }
    cnt++;
  }
  return EnemyCommand(makeEnemyCommandType(command), splited);
}
