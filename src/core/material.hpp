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
  
#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include "color.hpp"

namespace GM {
    class Material {
        public:
            Material();
            Material(Color, float, float, float);
            void setColor(const Color&);
            void setDiffuse(float);
            void setSpecular(float);
            void setReflection(float);
//            void setRefraction(float);
            Color getColor();            
            float getDiffuse();
            float getSpecular();
            float getReflection();
//            float getRefraction();            
        private:
            Color rgba;
            float diffuse;
            float specular;
//            float refraction;
            float reflection;            
    };
    
    inline Material::Material() {}
    
    inline float Material::getDiffuse() {
        return diffuse;
    }
    
    inline float Material::getSpecular() {
        return specular;
    }

    inline float Material::getReflection() {
        return reflection;
    }

/*
    inline float Material::getRefraction() {
        return refraction;
    }
*/

    inline Material::Material(Color c, float df, float sf, float rf):
        rgba(c), diffuse(df), specular(sf), reflection(rf) {}
    
    inline void Material::setColor(const Color& c) {
        rgba = c;
    }

    inline void Material::setDiffuse(float f) {
        diffuse = f;
    }

    inline void Material::setSpecular(float f) {
        specular = f;
    }
       
    inline void Material::setReflection(float f) {
        reflection = f;
    }
/*
    inline void Material::setRefraction(float f) {
        refraction = f;
    }
*/
    inline Color Material::getColor() {
        return rgba;
    }
    
}

#endif /*MATERIAL_HPP_*/
