/**
 * Plane
 */

#ifndef PLANE_HPP
#define PLANE_HPP

#include "common.hpp"
#include "object.hpp"
#include "point.hpp"
#include "vector.hpp"


namespace GM {
  class Plane : public Object {
  public:
    Plane(Point, Point, Point);
    VIRTUAL int serialize(void*);
    VIRTUAL void initBbox();
    VIRTUAL Vector getNormal(Point p);
  private:
    float a, b, c;
    Vector normal;
    Point lb, rt;
  };
}

#endif
