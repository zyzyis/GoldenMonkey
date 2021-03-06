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
#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "common.hpp"


namespace GM {
    /**
     * Abstract Light class
     */
    class Light {
        public:        
            Light(const Point&, float);
            Light(const Point&);            
            float intensity;
            Point pos;           
            Color c;
            void serialize(spu_light_t*);
    };
            
    inline Light::Light(const Point& p, float c)
        : intensity(c), pos(p), c(1.0f, 1.0f, 1.0f) {}

    inline Light::Light(const Point& p) 
        : intensity(1.0f), pos(p), c(1.0f, 1.0f, 1.0f) {}
        
    inline void Light::serialize(spu_light_t* ptr) {
        pos.serialize(ptr);
	ptr -> intensity = intensity;
    }    
}



#endif /*LIGHT_HPP_*/
