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
#include "grid.hpp"
#include "jobber/gridjob.hpp"
#include <cmath>
using namespace std;

namespace GM
{
    Grid::Grid(int factor) : gridFactor(factor)
	{}
	
    Grid::~Grid() {stop();}
  
  int Grid::updateScene() {
    int n = Global::objects.size();        
    nVoxel = gridFactor * gridFactor * gridFactor * n;
    voxels.resize(nVoxel);
          
    // unit all the objects and compute the bounding box
    Global::ObjectIterator it = Global::objects.begin();
    bbox = (*it) -> getBbox();
    
    while (it != Global::objects.end()) {    
      bbox.unit((*it)->getBbox());
      it ++;
    }

    float coef = 1.f / ( gridFactor * pow(n, 0.33333f));
    widthX = ( bbox.rt.x - bbox.lb.x ) * coef;
    widthY = ( bbox.rt.y - bbox.lb.y ) * coef;
    widthZ = ( bbox.lb.z - bbox.rt.z ) * coef;
    
    // update the overlap index for each voxel
    int x, y, z, x1, y1, z1;
    int i, j, k, c;
    int index;
    Bbox tmp;
    for (c = 0; c < n; c ++) {
      tmp = Global::objects[c] -> getBbox();
      point2vox(tmp.lb, x, y, z);
      point2vox(tmp.rt, x1, y1, z1);
      
      for (i = x; i <= x1; i ++) {
        for (j = y; j <= y1; j ++) {
          for (k = z; k <= z1; k++) {
            vox2index(i, j, k, index);
            voxels[index] |= (1u << c);
          }
        }} // end for      
    } // end for
    return 0;
  }
  
  int Grid::point2vox(const Point& p, int& x, int& y, int& z) 
  {
      if (!bbox.isInside(p))
        return 1;
      
      x = (int)((p.x - bbox.lb.x) / widthX);
      y = (int)((p.y - bbox.lb.y) / widthY);
      z = (int)((bbox.lb.z - p.z) / widthZ);
      
      return 0;      
  }

  int Grid::vox2index(int x, int y, int z, int& n) {
  	if ( x >= nVoxel || y >= nVoxel || z >= nVoxel )
  		return -1;
  	
  	n = x + y * nVoxel * nVoxel + z * nVoxel;
  	return 0;
  }
  
  int Grid::point2index(const Point& p, int& r) {
    if (!bbox.isInside(p))
      return 1;
      
    int xn = (int)((p.x - bbox.lb.x) / widthX);
    int yn = (int)((p.y - bbox.lb.y) / widthY);
    int zn = (int)((p.z - bbox.lb.z) / widthZ);
    vox2index(xn, yn, zn, r);
    return 0;
  }
  
  /**
   * Whenever the size of the queue is over the limit size of the grid
   * job, it will call the createJob function and push the new job into
   * the respective jobber.
   * @r the new ray.
   */
  int Grid::run() {
    while(1) {
      if (isStop) Thread::thread_exit();  
      
      if (Global::rayQueue.size() < GridJob::MAX_RAYSNO) {
        PUTS("Grid is waiting for ray...");
        thread_cond_wait(&Global::condRay, &Global::mutexRay);
        PUTS("Grid is active!");
      }
      
      if (isStop) Thread::thread_exit();
    	Global::global_add_job(new GridJob());
    }
  	return 0;
  }
  

}
