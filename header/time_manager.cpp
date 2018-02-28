#include "time_manager.h"

are_time::TimeManager::TimeManager() {
  this->diff = 0;
  this->prevTime = 0;
  this->nowTime = 0;
}

are_time::TimeManager::TimeManager(double nowTime) {
  this->prevTime = 0;
  this->nowTime = nowTime;
  this->diff = nowTime;
}

void are_time::TimeManager::Update(double nowTime) {
  this->prevTime = nowTime;
  this->nowTime = nowTime;
  this->diff = (this->nowTime) - (this->prevTime);
}

double are_time::TimeManager::GetDiff(){
  return this->diff;
}

double are_time::TimeManager::GetPrev() {
  return this->prevTime;
}

double are_time::TimeManager::GetNow() {
  return this->nowTime;
}
