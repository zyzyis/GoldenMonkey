#include "reduceintegrator.hpp"

#ifdef PROFILE
extern double spu_time;
#endif

namespace GM {

  /**
   * Return Tuple Format:
   * 0 - obj id
   * 1 - origin
   * 2 - diffuse coefficient
   * 3 - reflection coefficient
   * 4 - specular coefficient
   */
  int ReduceIntegrator::shading(void *ptr) {
    float *p = (float *)ptr;

    uint32_t tmp, obj;
    uint32_t origin;
    Material m;
    int i = 0;

    while(1) {
      // printf("%x %d %f %f %f\n", (int)p[i], (int)p[i + 1], p[i + 2], p[i + 3], p[i + 4]);
      if (p[i] == -1.0f) {
#ifdef PROFILE
	spu_time += p[i + 1];
#endif       
	break;
      }

      tmp = (uint32_t)p[i];
      
      origin  = (uint32_t)p[i + 1];
      obj = tmp & 0x0000FFFFu;
      if (obj != 0) {
	m = Global::objects[obj - 1] -> getMaterial();
	image[origin] = m.getColor() * 
	  (m.getDiffuse() * p[i + 2] + p[i + 4] * m.getSpecular());
      }

      obj = (tmp >> 16) & 0x0000FFFFu;
      if (obj != 0) {
	m = Global::objects[obj - 1] -> getMaterial();	
	image[origin] += m.getColor() * (m.getReflection() * p[i + 3]);
      }

      i += 5;
    }

    MemoryTool::free(p);
    return 0;
  }
}
