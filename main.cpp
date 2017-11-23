#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <complex>
#include <string>

using namespace std;

using Point2d = complex<double>;

class Circle {
private:
  Point2d c;
  double r;

public:
  Circle(double x, double y, double r) {
    this->c = Point2d{x, y};
    this->r = r;
  }
  Point2d Center() const {return c;}
  double Radius() const {return r;}
};

class Polygon2d {
  protected:
  vector<Point2d> v;

  public:

  Polygon2d(vector<Point2d> vertices) : v(vertices) {}
  
  void Draw() {
    glBegin(GL_POLYGON);
    for (auto vertex : v) {
      glVertex2d(vertex.real(), vertex.imag());
    }
    glEnd();
  }

  string Show() {
    string ret = "";
    for (auto vertex : v) {
      ret += "(%f, %f)\n";
    }
    return ret;
  }
};

//make Polygon2d methods
class Polygon2dMaker {
public:
  Polygon2d Circle2Polygon2d(const Circle& circle, int p = 180) {
    vector<Point2d> v = vector<Point2d>{};
    double x = circle.Center().real();
    double y = circle.Center().imag();
    double r = circle.Radius();
    
    for (int i = 0;i < p;i++) {
      double rad = 2 * M_PI / p * i;
      v.push_back(Point2d{x + r * cos(rad), y + r * sin(rad)});
    }
    return Polygon2d{v};
  }
};

void keyboard(unsigned char key, int x, int y);
void display(void);
void idle(void);


int main(int argc, char** argv)
{
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(640, 480);
  glutInit(&argc, argv);
  glutCreateWindow("GLUT Test");
  glutKeyboardFunc(&keyboard);
  glutIdleFunc(&idle);
  glutDisplayFunc(&display);

  glutMainLoop();

  return EXIT_SUCCESS;
}


void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case '\x1B':
      exit(EXIT_SUCCESS);
      break;
  }
}


void display()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(1.0f, 0.0f, 0.0f);

  Circle c = Circle{0.0, 0.0, 0.2};
  Polygon2dMaker maker = Polygon2dMaker{};
  Polygon2d polCircle = maker.Circle2Polygon2d(c, 180);
  polCircle.Draw();
  glutSwapBuffers();
}

void idle(void)
{
  glutPostRedisplay();
}
