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

#ifndef COMMON_HPP
#define COMMON_HPP

extern "C" {
#include <stdint.h>
#include <stdio.h>
#include <error.h>
}


#define RETURN_OK 0
#define RETURN_FAIL 1
#define INFINITY_MIN 1e-8

#define VIRTUAL
#define MULTITHREADS

#ifdef CBE
#define ALIGN __attribute__ ((aligned(16)))
#define _round16(s) (((int)((s) / 16) + 1) * 16)
#else
#define ALIGN
#define _round16(s)
#endif

#ifdef STATUS_PRINT
#define PUTS(s) fprintf(stderr, "%s\n", s);
#define PRINTF(...) fprintf(stderr, __VA_ARGS__)
#else
#define PUTS(s)
#define PRINTF(...)
#endif 

#define ERROR(s) perror(s)

#define _max(a, b) ((a) > (b) ? (a) : (b))
#define _min(a, b) ((a) < (b) ? (a) : (b)) 

enum {
  DIFFUSE_RAY,
  REFLECTION_RAY,
  REFRACTION_RAY
};

enum {
  JOB_GENERIC = 6,
  JOB_VECTOR
};


#define TIMEBASE 79800000.0
#define __time__(x) __asm__ volatile ("mftb %0" : "=r"(x))
#define clock2time(x) ((double)(x) / TIMEBASE * 1.0e3)

#endif /*GM_HPP*/
