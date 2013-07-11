#ifndef GETNORMAL_H
#define GETNORMAL_H

#include "primitive.h"

typedef spu_vector_t (*GetNormalFunc)(spu_shape_t, spu_point_t);
typedef void (*SIMD_GetNormalFunc)(float, float, float, float*, float*, float*, spu_shape_t);

spu_vector_t getnormal_sphere(spu_shape_t, spu_point_t);
spu_vector_t getnormal_plane(spu_shape_t, spu_point_t);

void simd_getnormal_sphere(float, float, float, float*, float*, float*, spu_shape_t);
void simd_getnormal_plane(float, float, float, float*, float*, float*, spu_shape_t);

#endif

