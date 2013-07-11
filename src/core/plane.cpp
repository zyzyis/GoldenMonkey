#include "plane.hpp"


namespace GM {
  Plane::Plane(Point p0, Point p1, Point p2)
    : Object(TYPE_PLANE), lb(p0), rt(p2) {
    initBbox();

    if (p0.x == p1.x && p1.x == p2.x) {
      a = -1.0f / p0.x;
    } else if (p0.y == p1.y && p1.y == p2.y) {
      b = -1.0f / p0.x;
    } else if (p0.z == p1.z && p1.z == p2.z) {
      c = -1.0f / p0.z;
    } else {
      // compute the plane equation and the normal
      float t = (p0.z * p2.x - p2.z * p0.x) * (p0.y * p1.x - p1.y * p0.x) - (p0.z * p1.x - p1.z * p0.x) * (p0.y * p2.x - p2.y * p0.x);
      c = (p1.x - p0.x) * (p0.y * p2.x - p2.y * p0.x) - (p2.x - p0.x) * (p0.y * p2.x - p2.y * p0.x);
      c /= t;

      t = p0.y * p2.x - p2.y * p0.x;
      b = c * (p1.z * p0.x - p0.z * p1.x) + (p0.x - p1.x);
      b /= t;

      a = (-1 - b * p0.y - c * p0.z ) / p0.x;
    }

    Vector v = p2 - p1;
    Vector u = p0 - p1;
    normal = v.cross(u);
    normal.normalize();
    /*
    printf("plane: A = %f B = %f C = %f\n", a, b, c);
    printf("normal: %f %f %f\n", normal.x, normal.y, normal.z);
    */
  }

  VIRTUAL int Plane::serialize(void *ptr) {
    if (ptr == NULL)
      return RETURN_FAIL;

    spu_shape_t *sp = (spu_shape_t*)ptr;

    sp -> type = TYPE_PLANE;
    sp -> shape.plane.a = a;
    sp -> shape.plane.b = b;
    sp -> shape.plane.c = c;
    sp -> shape.plane.nx = normal.x;
    sp -> shape.plane.ny = normal.y;
    sp -> shape.plane.nz = normal.z;

    return RETURN_OK;
  }

  VIRTUAL Vector Plane::getNormal(Point p) {
    return normal;
  }

  VIRTUAL void Plane::initBbox() {
    bbox.lb = lb;
    bbox.rt = rt;
  }
}
