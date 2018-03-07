#pragma once
#include <random>

class RandomUtil
{
  std::mt19937 engine;
  
 public:
  RandomUtil();
  int Range(int minVal, int maxVal);
};
