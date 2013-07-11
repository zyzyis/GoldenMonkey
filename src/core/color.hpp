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
#ifndef COLOR_HPP_
#define COLOR_HPP_

#include <ImfRgba.h>
using Imf::Rgba;

namespace GM {

  class Color {
  public:
    Color(): r(0.0f), g(0.0f), b(0.0f), a(1.0f) {}
    Color(float r_, float g_, float b_):
      r(r_), g(g_), b(b_), a(1.0f) {}
    Rgba toRgba();

    Color operator+ (const Color& c);
    Color operator* (float f);
    Color operator+= (const Color& c);
    Color operator* (const Color& c);
    float r, g, b, a;
  };
    
  inline Rgba Color::toRgba() {
    Rgba rgba;
    rgba.r = r;
    rgba.g = g;
    rgba.b = b;
    rgba.a = a;
    return rgba;
  }

  inline Color Color::operator+ (const Color& c) {
    return Color(r + c.r, g + c.g, b + c.b);
  }
    
  inline Color Color::operator* (float f) {
    return Color(r * f, g * f, b * f);
  }
    
  inline Color Color::operator+= (const Color& c) {
    r += c.r; g += c.g; b += c.b;
    return *this; 
  }
    
  inline Color Color::operator* (const Color& c) {
    return Color(r * c.r, g * c.g, b * c.b);
  }
} 

#endif /*COLOR_HPP_*/
