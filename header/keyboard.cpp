#include "keyboard.h"

bool Keyboard::IsPressed(key_type key) {
  return pressed.find(key) != pressed.end();
}
bool Keyboard::IsHeld(key_type key){
  return IsPressed(key) || held.find(key) != held.end();
}
void Keyboard::Update(){
  pressed.clear();
}
void Keyboard::Press(key_type key) {
  pressed.insert(key);
  held.insert(key);
}
void Keyboard::Release(key_type key) {
  held.erase(key);
}
