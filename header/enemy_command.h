#pragma once

#include <vector>
#include <string>
/*
(move t angle v)
(pmove t angle v)
(stop t) = (move t 0 0)
(attack id)
*/

enum class EnemyCommandType{move, pmove, stop, attack, nop};

class EnemyCommand
{
 public:
  
 EnemyCommand(EnemyCommandType command, const std::vector<std::string>& args);
 std::vector<std::string> args;
 EnemyCommandType command;
};

EnemyCommandType makeEnemyCommandType(std::string str);

EnemyCommand makeEnemyCommand(std::string raw);
