#pragma once

#include <memory>
#include <unordered_set>

class Keyboard {
  std::unordered_set<unsigned char> pressed;
  std::unordered_set<unsigned char> held;

public:
  bool IsPressed(unsigned char key); 
  bool IsHeld(unsigned char key);
  void Update();
  void Press(unsigned char key);
  void Release(unsigned char key);
};

extern std::unique_ptr<Keyboard> keyboard;
