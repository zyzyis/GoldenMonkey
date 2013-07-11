/**
 * Main File
 * The start file of the golden monkey render.
 * Author: Si Yin
 */
#include <cstdlib>
#include <sys/types.h>
#include "gm.hpp"
using namespace GM;
using namespace std;

#ifdef PROFILE
uint64_t time_start, init_end, time_end;
extern uint32_t nspu;
double spu_time;
#endif

extern GMrender* parseScene(const char*);

int main(int argc, char** argv) {
  
#ifdef PROFILE
  __time__(time_start);
  spu_time = 0.0;
#endif

  if (argc != 3) {
    fprintf(stderr, "gm [input file] [output file]\n");
    return 1;
  }
    
  GMrender *render = parseScene(argv[1]);
  if (render == NULL)
    return 1;

  render -> render();
    
  render -> writeToFile(argv[2]);

  delete render;

#ifdef PROFILE
  __time__(time_end);
  fprintf(stderr, "%lf %lf %lf\n", clock2time(time_end - time_start), 
	  clock2time(init_end - time_start), clock2time(spu_time / (double)nspu));
#endif
  return 0;
}
