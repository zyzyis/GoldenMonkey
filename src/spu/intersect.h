#ifndef INTERSECT_H
#define INTERSECT_H

#include "primitive.h"
#include <math.h>
#include <simdmath/sqrtf4.h>

typedef int (*IntersectFunc)(spu_ray_t, spu_shape_t, float*);

typedef vec_float4 (*SIMD_IntersectFunc) (vec_float4,
					  vec_float4,
					  vec_float4,
					  vec_float4,
					  vec_float4,
					  vec_float4,
					  spu_shape_t);

int intersect_sphere(spu_ray_t, spu_shape_t, float*);
int intersect_plane(spu_ray_t, spu_shape_t, float*);

vec_float4 simd_intersect_sphere (vec_float4,
				  vec_float4,
				  vec_float4,
				  vec_float4,
				  vec_float4,
				  vec_float4,
				  spu_shape_t);

vec_float4 simd_intersect_plane (vec_float4,
				 vec_float4,
				 vec_float4,
				 vec_float4,
				 vec_float4,
				 vec_float4,
				 spu_shape_t);
#endif
