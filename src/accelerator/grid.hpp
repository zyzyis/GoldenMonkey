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
#ifndef GRID_HPP_
#define GRID_HPP_

#include "accelerator.hpp"
#include "core/bbox.hpp"
#include "core/object.hpp"
#include "util/global.hpp"
#include "common.hpp"
extern "C" {
#include <stdint.h>
}

namespace GM
{
  class Grid : public Accelerator {
  public:
    typedef vector<int>::iterator VoxelIterator; 
    typedef uint64_t BitFlag;
    /**
     * Constructor
     * @param Grid factor is used to decide the density of the grid.
     */
     Grid(int factor);
    	
     /**
      * destructor
      */     
     ~Grid();
     
     /**
      * virtual function. Update the scene whenever data is changed
      */
     int updateScene();
     
     /**
      * point to voxel index
      */
     int point2vox(const Point&, int&, int&, int&);
     
     /**
      * point to index number
      */
     int point2index(const Point&, int&);    
     
     /**
      * voxel to index number
      */
     int vox2index(int, int, int, int&);

     /**
      * get the bounding box.
      */
     Bbox getBbox();
     
     /**
      * Get the voxel number per axis.
      * @return the number of voxel for each axis.
      */
     int getVoxelNo();
     
     /**
      * Index the vector faster using operator overload.
      */
     BitFlag operator() (int i);

      /**
       * Detect the rays pool and create new job when the size
       * of the queue is beyond the limit. 
       */
      VIRTUAL int run();
     
     /**
      * Stop the thread 
      */
      VIRTUAL int stop();
     
    protected:
      vector<BitFlag> voxels;
      float widthX;
      float widthY;
      float widthZ;
      int nVoxel;
      int gridFactor;
      Bbox bbox;
  };

  inline int Grid::getVoxelNo() { 
    return nVoxel;
  }
   
  inline Bbox Grid::getBbox() {
    return bbox;
  }
  
  inline Grid::BitFlag Grid::operator() (int i) {
  	return voxels[i];
  }
  
}

#endif /*GRID_H_*/
