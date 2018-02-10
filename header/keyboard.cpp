#include "keyboard.h"

bool Keyboard::IsPressed(unsigned char key) {
  return pressed.find(key) != pressed.end();
}
bool Keyboard::IsHeld(unsigned char key){
  return IsPressed(key) || held.find(key) != held.end();
}
void Keyboard::Update(){
  pressed.clear();
}
void Keyboard::Press(unsigned char key) {
  pressed.insert(key);
  held.insert(key);
}
void Keyboard::Release(unsigned char key) {
  held.erase(key);
}
