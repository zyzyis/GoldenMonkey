#include "shading.h"
#include "testsuite.h"

vec_float4 length2(vec_float4 x, vec_float4 y, vec_float4 z) {
  return spu_madd(x, x, spu_madd(y, y, spu_mul(z, z)));
}

vec_float4 length(vec_float4 x, vec_float4 y, vec_float4 z) {
  return _sqrtf4(spu_madd(x, x, spu_madd(y, y, spu_mul(z, z))));
}

void normalize(vec_float4 *x, vec_float4 *y, vec_float4 *z) {
  vec_float4 len = length(*x, *y, *z);
  len = spu_re(len);

  (*x) = spu_mul(*x, len);
  (*y) = spu_mul(*y, len);
  (*z) = spu_mul(*z, len);
}

vec_float4 vec_dot(vec_float4 x1, vec_float4 y1, vec_float4 z1,
		   vec_float4 x2, vec_float4 y2, vec_float4 z2) {
  return spu_madd(x1, x2, spu_madd(y1, y2, spu_mul(z1, z2)));
}

/**
 * Shading diffuse color in vector.
 */
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
		     float* out,        // the tuple output
		     int k) {           // the output position
  vec_float4 dist;
  vec_float4 vec_zero = spu_splats(0.0f);
  mix_uint4 flag;
  mix_float4 diffuse, dot;
  int i, pos;

  dist = spu_re(length2(shdx, shdy, shdz));

  normalize(&shdx, &shdy, &shdz);
  normalize(&normx, &normy, &normz);

  // TODO: specular can be added here with a power function
  dot.v = vec_dot(shdx, shdy, shdz, normx, normy, normz);

  // get the diffuse color
  diffuse.v = spu_mul(bright, spu_mul(dot.v, dist));

  // mask == 0 && objid > 0 && dot > 0
  flag.v = spu_and(spu_cmpeq(mask, spu_splats(0u)), spu_cmpgt(objid.v, spu_splats(0u)));
  flag.v = spu_and(flag.v, spu_cmpgt(dot.v, spu_splats(0.0f)));

  // add new tuple
  for (i = 0; i < 4; i ++) {    
    pos = (k + i) * 5;
    out[pos] = 0.0f;
    if (__builtin_expect((flag.a[i] != 0), 0)) {
      out[pos]     = objid.a[i];
      out[pos + 1] = origin.a[i];
      out[pos + 2] = diffuse.a[i]; 
      // printf("diffuse: %d %d %d %f\n", pos, objid.a[i] - 1, origin.a[i], diffuse.a[i]);
    }
  }
}

void shading_specular(vec_uint4 mask,
		      vec_uint4 objid,
		      vec_float4 idx,
		      vec_float4 idy,
		      vec_float4 idz,
		      vec_float4 odx,
		      vec_float4 ody,
		      vec_float4 odz,
		      float *out,
		      int k) {
  mix_float4 dot;
  mix_uint4 flag;
  int i;
  
  normalize(&idx, &idy, &idz);
  normalize(&odx, &ody, &odz);
  
  dot.v = vec_dot(idx, idy, idz, odx, ody, odz);

  // mask == 0 && objid > 0 && dot > 0
  flag.v = spu_and(spu_cmpeq(mask, spu_splats(0u)), spu_cmpgt(objid, spu_splats(0u)));
  flag.v = spu_and(flag.v, spu_cmpgt(dot.v, spu_splats(0.0f)));

  for (i = 0; i < 4; i ++) {
    if (__builtin_expect((flag.a[i] != 0), 0)) {
      out[(k + i) * 5 + 4] = pow(dot.a[i], 20.0f);
    }
  }
}

/**
 * Shading reflection color in vector
 */
void shading_reflection(mix_uint4 objid,   // intersect
			mix_uint4 origin,  // the image position
			int round,         // the recursion level
 			float* out,        // the tuple output
			int k) {           // the output position    
  int i, pos;
  uint32_t tmp;
  // CAUTION: the maxmize reflection level support is only 1 at this moment.
  float coef = 1.0f;

  for (i = 0; i < 4; i ++) {
    if (__builtin_expect((objid.a[i] != 0), 0)) {
      pos = (k + i) * 5;
      // the high 16 bits are for the reflection object
      tmp = (uint32_t)out[(k + i) * 5];
      tmp |= ((objid.a[i] << 16) & 0xFFFF0000);
      // printf("origin: %d robjid = %d\n", (uint32_t)out[(k + i) * 5], objid.a[i]);
      out[pos] = (float)tmp;
      out[pos + 3] = coef;
    }
  }
}
