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

/* Definition */
%{

#include "y.tab.h"
int line_num;
int str_pos;

%}

WHITESPACE [ \t]+
NUMBER [-+]?([0-9]+|(([0-9]+\.[0-9]*)|(\.[0-9]+)))([eE][-+]?[0-9]+)?
IDENT [a-zA-Z_][a-zA-Z_0-9]*

%x STR COMMENT
%option noyywrap
%option never-interactive

/* Rules */
%%
"#" { BEGIN COMMENT; }
<COMMENT>. /* eat it up */
<COMMENT>\n { line_num++; BEGIN INITIAL; }

Accelerator		{ return ACCELERATOR;           }
AreaLightSource		{ return AREALIGHTSOURCE;	}
AttributeBegin 		{ return ATTRIBUTEBEGIN; 	}
AttributeEnd 		{ return ATTRIBUTEEND; 		}
Camera			{ return CAMERA;		}
ConcatTransform 	{ return CONCATTRANSFORM; 	}
CoordinateSystem	{ return COORDINATESYSTEM;	}
CoordSysTransform	{ return COORDSYSTRANSFORM;	}
Film	 		{ return FILM;	 		}
Identity		{ return IDENTITY;		}
LightSource 		{ return LIGHTSOURCE;		}
LookAt			{ return LOOKAT;		}
Material		{ return MATERIAL;		}
ObjectBegin 		{ return OBJECTBEGIN;		}
ObjectEnd 		{ return OBJECTEND;		}
ObjectInstance 		{ return OBJECTINSTANCE;	}
PixelFilter 		{ return PIXELFILTER; 		}
ReverseOrientation 	{ return REVERSEORIENTATION;	}
Rotate			{ return ROTATE;		}
Sampler			{ return SAMPLER;		}
SearchPath		{ return SEARCHPATH;		}
Scale 			{ return SCALE;			}
Shape			{ return SHAPE;			}
SurfaceIntegrator	{ return SURFACEINTEGRATOR;	}
Texture			{ return TEXTURE;		}
TransformBegin  	{ return TRANSFORMBEGIN;	}
TransformEnd	        { return TRANSFORMEND;	        }
Transform		{ return TRANSFORM;		}
Translate		{ return TRANSLATE; 		}
Volume			{ return VOLUME;		}
VolumeIntegrator	{ return VOLUMEINTEGRATOR;	}
WorldBegin 		{ return WORLDBEGIN; 		}
WorldEnd 		{ return WORLDEND;		}

{WHITESPACE} /* do nothing */

\n { line_num++; }

{NUMBER} { return NUM; }

{IDENT} { return ID; }

"[" { return LBRACK; }
"]" { return RBRACK; }

\" { BEGIN STR; str_pos = 0; }
<STR>\"   { BEGIN INITIAL; return STRING;   }
<STR>.    { str_pos ++; }
<STR>\n   { fprintf(stderr, "Unterminated string!"); }

. { printf( "Illegal character: %c",yytext[0] ); }
%%

/* int main(int argc, char** argv) { */
/*   int result; */

/*   if ( argc > 1 ) */
/*     yyin = fopen(argv[1], "r"); */
/*   else */
/*     yyin = stdin; */

/*   while( result = yylex()) */
/*   printf("%d\n", result); */
  
/*   return 0; */
/* }   */
