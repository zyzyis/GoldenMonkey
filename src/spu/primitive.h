/*
 * Copyright 2007 Si Yin. <Golden Monkey>
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. 
 */
#ifndef PRIMITIVE_H 
#define PRIMITIVE_H
#include <stdint.h>

typedef struct {
  /*
  float x1, y1, z1;
  float x2, y2, z2;
  float x3, y3, z3;
  */
  float a, b, c;
  float nx, ny, nz;
} spu_plane_t;

typedef struct {
  float x, y, z;
} spu_point_t;

typedef struct {
  float x, y, z, intensity;
} spu_light_t;

typedef spu_point_t spu_vector_t;

typedef struct {
  spu_point_t o;
  spu_vector_t d;
  uint8_t type;    
  uint8_t round;
  uint32_t origin;
  float coef;
} spu_ray_t;

typedef struct {
  spu_ray_t rr;
  uint32_t shade;    
  uint32_t objid;    
} spu_intersection_t;

typedef struct {
  spu_point_t o;
  float radius;
} spu_sphere_t;

typedef struct {
  int type;
  union {
    spu_sphere_t sphere;
    spu_plane_t plane;
  } shape;
} spu_shape_t;

enum {
  TYPE_SPHERE,
  TYPE_PLANE,
};

enum {
  NORMAL_INSIDE,
  NORMAL_OUTSIDE,
  NORMAL_LEFT,
  NORMAL_RIGHT,
  NORMAL_UP,
  NORMAL_DOWN
};

/*
typedef union {
  float a[4];
  __vector float v;
} mix_float4;

typedef union {
  unsigned int a[4];
  __vector unsigned int v;
} mix_uint4;
*/
#endif
