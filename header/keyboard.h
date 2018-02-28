#pragma once

#include <memory>
#include <unordered_set>

class Keyboard {
  using key_type = int;
  std::unordered_set<key_type> pressed;
  std::unordered_set<key_type> held;

public:
  bool IsPressed(key_type key); 
  bool IsHeld(key_type key);
  void Update();
  void Press(key_type key);
  void Release(key_type key);
};

extern std::unique_ptr<Keyboard> keyboard;
