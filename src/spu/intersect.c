#include "intersect.h"
#include "testsuite.h"
#include <stdio.h>


#define _pow2(x) ((x) * (x)) 
/*
#define _x1(x) x.shape.plane.x1
#define _y1(x) x.shape.plane.y1
#define _z1(x) x.shape.plane.z1
#define _x2(x) x.shape.plane.x2
#define _y2(x) x.shape.plane.y2
#define _z2(x) x.shape.plane.z2
#define _x3(x) x.shape.plane.x3
#define _y3(x) x.shape.plane.y3
#define _z3(x) x.shape.plane.z3
*/

/**
 * Intersect sphere function
 */
int intersect_sphere(spu_ray_t ray, 
                     spu_shape_t shape, 
                     float* rayt) { 
  spu_point_t o  = ray.o;
  spu_vector_t d = ray.d;
  spu_point_t c  = shape.shape.sphere.o;
  float r = shape.shape.sphere.radius;
  float a = _pow2(d.x) + _pow2(d.y) + _pow2(d.z);
  float b = 2 * (o.x * d.x + o.y * d.y + o.z * d.z 
		 - c.x * d.x - c.y * d.y - c.z * d.z);
  float e = _pow2(o.x - c.x) + _pow2(o.y - c.y) + _pow2(o.z - c.z) - _pow2(r);

  // solve the quadratic equation
  float delta = _pow2(b) - 4 * a * e;
  if (delta > 0.0f) {
    *rayt = (-b - sqrtf(delta)) / (2.f * a);
    return 1;
  }
  return 0;
}

/**
 *  Intersect plane function
 */
int intersect_plane(spu_ray_t ray,
		    spu_shape_t s,
		    float* rayt) {

  float a = s.shape.plane.a;
  float b = s.shape.plane.b;
  float c = s.shape.plane.c;

  float t = a * ray.d.x + b * ray.d.y + c * ray.d.z;
  if (t == 0)
    return 0;
  
  (*rayt) = (-1 - a * ray.o.x - b * ray.o.y - c * ray.o.z) / t;
  return 1;
}

vec_float4 simd_intersect_sphere(vec_float4 ox,
				 vec_float4 oy,
				 vec_float4 oz,
				 vec_float4 dx,
				 vec_float4 dy,
				 vec_float4 dz,
				 spu_shape_t shape) {

  vec_float4 e, t, delta, rs;
  vec_uint4 flag;
  // vectorize the sphere structure
  vec_float4 r   = spu_splats(shape.shape.sphere.radius);
  vec_float4 sox = spu_splats(shape.shape.sphere.o.x);
  vec_float4 soy = spu_splats(shape.shape.sphere.o.y);
  vec_float4 soz = spu_splats(shape.shape.sphere.o.z);

  vec_float4 a = spu_madd(dx, dx, spu_madd(dy, dy, spu_mul(dz, dz)));
  vec_float4 b = spu_madd(ox, dx, spu_madd(oy, dy, spu_mul(oz, dz)));
  b = spu_sub(b, spu_madd(sox, dx, spu_madd(soy, dy, spu_mul(soz, dz))));
  b = spu_mul(spu_splats(2.0f), b);

  t = spu_sub(ox, sox);
  e = spu_mul(t, t);
  t = spu_sub(oy, soy);
  e = spu_madd(t, t, e);
  t = spu_sub(oz, soz);
  e = spu_madd(t, t, e);
  e = spu_sub(e, spu_mul(r, r));
  
  t = spu_mul(spu_splats(4.0f), spu_mul(a, e));
  delta = spu_msub(b, b, t);
  flag = spu_cmpgt(delta, spu_splats(0.0f));
  rs = spu_add(b,  _sqrtf4(delta));
  rs = spu_mul(rs, spu_re(spu_mul(spu_splats(-2.0f), a)));
  
  rs = spu_sel(spu_splats(0.0f), rs, flag);

  return rs;
}


vec_float4 simd_intersect_plane(vec_float4 ox,
				vec_float4 oy,
				vec_float4 oz,
				vec_float4 dx,
				vec_float4 dy,
				vec_float4 dz,
				spu_shape_t s) {

  vec_float4 a = spu_splats(s.shape.plane.a);
  vec_float4 b = spu_splats(s.shape.plane.b);
  vec_float4 c = spu_splats(s.shape.plane.c);

  vec_float4 t = spu_madd(a, dx, spu_madd(b, dy, spu_mul(c, dz)));
  vec_float4 k = spu_madd(a, ox, spu_madd(b, oy, spu_mul(c, oz)));
  k = spu_sub(spu_splats(-1.0f), k);
  k = spu_mul(k, spu_re(t));
  return k;
}
