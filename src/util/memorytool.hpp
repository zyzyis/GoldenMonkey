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

#ifndef MEMORYTOOL_HPP
#define MEMORYTOOL_HPP

#include "common.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace GM {
  class MemoryTool {
  public:
    static void* alloc(size_t s, const char *when = NULL);
    static int free(void* p, const char *when = NULL);
    static int copy(void* src, void* des, int size);
  };

  inline void *MemoryTool::alloc(size_t s, const char *when) {
    if (when != NULL)
      PRINTF("memory alloc : %d bytes at %s\n", (int)s, when);
    return std::malloc(_round16(s));
  }

  inline int MemoryTool::copy(void* src, void* des, int size) {
    std::memcpy(des, src, size);
    return RETURN_OK;
  }

  inline int MemoryTool::free(void* p, const char *when) {
    if (when != NULL)
      PRINTF("free by %s\n", when);
    std::free(p);
    return RETURN_OK;
  }

}

#endif /*MEMORYTOOL_*/
