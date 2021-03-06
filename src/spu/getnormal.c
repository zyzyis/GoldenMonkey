#include "getnormal.h"
#include "primitive.h"

/**
 * Obtain the normal of the intersection point at a sphere.
 */
spu_vector_t getnormal_sphere(spu_shape_t shape, spu_point_t p) {
  spu_vector_t rval;
  rval.x = p.x - shape.shape.sphere.o.x;
  rval.y = p.y - shape.shape.sphere.o.y;
  rval.z = p.z - shape.shape.sphere.o.z;
  return rval;
}

spu_vector_t getnormal_plane(spu_shape_t shape, spu_point_t p) {
    spu_vector_t rval;
	
    rval.x = shape.shape.plane.nx;
    rval.y = shape.shape.plane.ny;
    rval.z = shape.shape.plane.nz;
  
  return rval;
}
void simd_getnormal_sphere(float x, float y, float z, 
			   float* nx, float* ny, float *nz,
			   spu_shape_t s) {
  (*nx) = x - s.shape.sphere.o.x;
  (*ny) = y - s.shape.sphere.o.y;
  (*nz) = z - s.shape.sphere.o.z;
}

void simd_getnormal_plane(float x, float y, float z, 
			  float* nx, float* ny, float *nz,
			  spu_shape_t s) {
  (*nx) = s.shape.plane.nx;
  (*ny) = s.shape.plane.ny;
  (*nz) = s.shape.plane.nz;

  /*
  float wx, wy, wz, vx, vy, vz;
  wx = s.shape.plane.x1 - s.shape.plane.x2;
  wy = s.shape.plane.y1 - s.shape.plane.y2;
  wz = s.shape.plane.z1 - s.shape.plane.z2;
  vx = s.shape.plane.x3 - s.shape.plane.x2;
  vy = s.shape.plane.y3 - s.shape.plane.y2;
  vz = s.shape.plane.z3 - s.shape.plane.z2;

  (*nx) = vy * wz - wy * vz;
  (*ny) = vz * wx - wz * vx;
  (*nz) = vx * wy - wx * vy;
  */
}
