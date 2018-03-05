#include "enemy_command.h"
#include <sstream>
#include <iostream>

EnemyCommand::EnemyCommand(EnemyCommandType command, const std::vector<std::string>& args) {
  this->command = command;
  this->args = args;
}

EnemyCommandType makeEnemyCommandType(const std::string& str) {
  if (str == "move") {
    return EnemyCommandType::move;
  } else if (str == "mangle") {
    return EnemyCommandType::mangle;
  } else if (str == "mpangle") {
    return EnemyCommandType::mpangle;
  } else if (str == "stop") {
     return EnemyCommandType::stop;
  } else if (str == "bullet") {
    return EnemyCommandType::bullet;
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
  auto commandType = makeEnemyCommandType(command);
  if (commandType == EnemyCommandType::bullet) {
    if ((int)splited.size() == 1) {
      splited.push_back("0");
      splited.push_back("0");
    }
  }
  return EnemyCommand(commandType, splited);
}
