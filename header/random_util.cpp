#include "random_util.h"

RandomUtil::RandomUtil() {
  std::random_device seed_gen;
  engine =  std::mt19937(seed_gen());
}

int RandomUtil::Range(int minVal, int maxVal) {
  return engine() % (maxVal - minVal + 1) + minVal;
}
