#pragma once

#include <vector>
#include <string>
/*
(mpangle relangle)
(mangle relangle)
(move t v)
(stop t) = (move t 0)
(attack id)
*/

enum class EnemyCommandType{move, mangle, mpangle, stop, attack, nop};

class EnemyCommand
{
 public:
  
 EnemyCommand(EnemyCommandType command, const std::vector<std::string>& args);
 std::vector<std::string> args;
 EnemyCommandType command;
};

EnemyCommandType makeEnemyCommandType(const std::string& str);

EnemyCommand makeEnemyCommand(std::string raw);
