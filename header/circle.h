#pragma once

#include <complex>
#include <GLFW/glfw3.h>
#include <cmath>

using Point2d = std::complex<double>;

class Circle {
private:
  static const int ACCURACY;
  Point2d c;
  double r;
  // TODO: have color as a field

public:
  Circle() = default;
  Circle(double x, double y, double r);
  Point2d Center() const;
  double Radius() const;
  void Draw();
  
  bool IsOverlapC(const Circle &x);
};
