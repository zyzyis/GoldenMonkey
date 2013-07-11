#ifndef SHADING_H
#define SHADING_H

#include <spu_intrinsics.h>
#include <simdmath/sqrtf4.h>
#include <math.h>

#include "primitive.h"

typedef union {
  float a[4];
  __vector float v;
} mix_float4;

typedef union {
  unsigned int a[4];
  __vector unsigned int v;
} mix_uint4;

vec_float4 length2(vec_float4, vec_float4, vec_float4);
vec_float4 length(vec_float4, vec_float4, vec_float4);
void normalize(vec_float4 *, vec_float4 *, vec_float4 *);

vec_float4 vec_dot(vec_float4, vec_float4, vec_float4,
		   vec_float4, vec_float4, vec_float4);

void shading_diffuse(vec_uint4 mask,    // the shadow ray mask
		     mix_uint4 objid,   // intersect object
		     mix_uint4 origin,  // the image position
		     vec_float4 shdx,   // shadow ray x
		     vec_float4 shdy,   // shadow ray y
		     vec_float4 shdz,   // shadow ray z
		     vec_float4 normx,  // normal x
		     vec_float4 normy,  // normal y
		     vec_float4 normz,  // normal z
		     vec_float4 bright, // the intensity
		     float* out, int);  // the tuple output

void shading_reflection(mix_uint4 objid,   // intersect
			mix_uint4 origin,  // the image position
			int round,         // the recursion level
 			float* out, int);  // the tuple output

void shading_specular(vec_uint4 mask,    // the shadow ray
		      vec_uint4 objid,   // intersected object
		      vec_float4 idx,    // the incident ray x
		      vec_float4 idy,    // the incident ray y		      
		      vec_float4 idz,    // the incident ray z
		      vec_float4 odx,    // the specular ray x
		      vec_float4 ody,    // the specular ray y
		      vec_float4 odz,    // the specular ray z
		      float *out,        // the output
		      int k);            // the tuple output

#endif
