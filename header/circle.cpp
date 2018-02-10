#include "circle.h"

Circle::Circle(double x, double y, double r){
  this->c = Point2d{x, y};
  this->r = r;
}
Point2d Circle::Center() const {
  return c;
}
double Circle::Radius() const {return r;}
void Circle::Draw() {
  // TODO: set color
  glBegin(GL_POLYGON);

  double x = Center().real();
  double y = Center().imag();
  double r = Radius();
  for (int i = 0;i < ACCURACY;i++) {
    double rad = 2 * M_PI / ACCURACY * i;
    glVertex2d(x + r * cos(rad), y + r * sin(rad));
  }
  glEnd();    
}
bool Circle::IsOverlapC(const Circle &x) {
  return abs(c - x.Center()) <= r + x.Radius();
}

const int Circle::ACCURACY = 180;
