#include <cstdio>
#include <cstdlib>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <sys/types.h>

#include "gm.hpp"
#include "GMcamera.hpp"
#include "GMsampler.hpp"
#include "GMaccel.hpp"
#include "GMcore.hpp"
#include "GMjobber.hpp"
#include "GMintegrator.hpp"
#include "common.hpp"

using namespace std;
using namespace GM;

#define MAX_SPU 6

uint32_t nspu;

bool hasName(xmlNodePtr cur, const char* str) {
  return !xmlStrcmp(cur -> name, (const xmlChar *)str);
}

int getOneFloat(xmlNodePtr cur, xmlDocPtr doc, float *v, const char *str) {
  char *txt;
  txt = (char *)xmlNodeListGetString(doc, cur -> children, 1);
  if (sscanf(txt, "%f", v) != 1) {
    fprintf(stderr, "Error: %s\n", str);
    xmlFree(txt);
    return 1;
  }
  xmlFree(txt);
  return 0;
}

int getTwoFloat(xmlNodePtr cur, xmlDocPtr doc, float *a, float *b, const char *str) {
  char *txt;
  txt = (char *)xmlNodeListGetString(doc, cur -> children, 1);
  if (sscanf(txt, "%f %f", a, b) != 2) {
    fprintf(stderr, "Error: %s\n", str);
    xmlFree(txt);
    return 1;
  }
  xmlFree(txt);
  return 0;
}

int getTripleFloat(xmlNodePtr cur, xmlDocPtr doc, 
		   float *a, float *b, float *c, 
		   const char *str) {
  xmlChar *txt;
  txt = xmlNodeListGetString(doc, cur -> children, 1);
  if (sscanf((const char*)txt, "%f %f %f", a, b, c) != 3) {
    fprintf(stderr, "Error: %s\n", str);
    xmlFree(txt);
    return 1;
  }
  xmlFree(txt);
  return 0;
}

/**
 * Parse the material entry.
 */
Material* parseMaterial(xmlNodePtr cur, xmlDocPtr doc) {
  float r, g, b;
  float diffuse, specular, reflection;

  cur = cur -> children;
  while(cur != NULL) {
    if (hasName(cur, "color"))
      if (getTripleFloat(cur, doc, &r, &g, &b, "material: color"))
	return NULL;

    if (hasName(cur, "diffuse"))
      if (getOneFloat(cur, doc, &diffuse, "material: diffuse"))
	return NULL;

    if (hasName(cur, "specular")) 
      if (getOneFloat(cur, doc, &specular, "material: specular"))
	return NULL;

    if (hasName(cur, "reflection"))
      if (getOneFloat(cur, doc, &reflection, "material: reflection"))
	return NULL;

    cur = cur -> next;
  }
#ifdef PARSEPRINT
  printf("material:\n");
  printf("rgb = %.2f %.2f %.2f\n", r, g, b);
  printf("dsr = %.2f %.2f %.2f\n", diffuse, specular, reflection);
#endif
  return new Material(Color(r, g, b), diffuse, specular, reflection);
}

/**
 * Caller should free the return object ptr
 */
Object* parseSphere(xmlNodePtr cur, xmlDocPtr doc) {
  float radius;
  float x, y, z;
  Material* m = NULL;

  cur = cur -> children;
  while(cur != NULL) {
    if (hasName(cur, "radius"))
      if (getOneFloat(cur, doc, &radius, "sphere: radius"))
	return NULL;

    if (hasName(cur, "point"))
      if (getTripleFloat(cur, doc, &x, &y, &z, "sphere: point"))
	return NULL;
    
    if (hasName(cur, "material")) {
      if ((m = parseMaterial(cur, doc)) == NULL)
	return NULL;
    }
    
    cur = cur -> next;
  }

  Object *sphere = new Sphere(Point(x, y, z), radius);
  sphere -> setMaterial(*m);
  delete m;
  
#ifdef PARSEPRINT
  printf("sphere:\n");
  printf("radius = %.2f\n", radius);
  printf("x = %.2f y = %.2f z = %.2f\n", x, y, z);
#endif
  return sphere;
}

Object* parsePlane(xmlNodePtr cur, xmlDocPtr doc) {
  float x1, x2, x3;
  float y1, y2, y3;
  float z1, z2, z3;
  Material *m = NULL;

  cur = cur -> children;
  while (cur != NULL) {
    if (hasName(cur, "point0"))
      if (getTripleFloat(cur, doc, &x1, &y1, &z1, "plane : point0"))
	return NULL;

    if (hasName(cur, "point1"))
      if (getTripleFloat(cur, doc, &x2, &y2, &z2, "plane : point1"))
	return NULL;

    if (hasName(cur, "point2"))
      if (getTripleFloat(cur, doc, &x3, &y3, &z3, "plane : point2"))
	return NULL;

    if (hasName(cur, "material"))
      if ((m = parseMaterial(cur, doc)) == NULL)
	return NULL;
    cur = cur -> next;
  }
	
  Object *plane = new Plane(Point(x1, y1, z1), Point(x2, y2, z2),
			    Point(x3, y3, z3));
  plane -> setMaterial(*m);
  delete m;
#ifdef PARSEPRINT
  printf("plane:\n");
  printf("x1 y1 z1 = %.2f %.2f %.2f\n", x1, y1, z1);
  printf("x2 y2 z2 = %.2f %.2f %.2f\n", x2, y2, z2);
  printf("x3 y3 z3 = %.2f %.2f %.2f\n", x3, y3, z3);
#endif
  return plane;
}

/**
 * parsing the light entry.
 */
Light* parseLight(xmlNodePtr cur, xmlDocPtr doc) {
  float px, py, pz;
  float intensity;

  cur = cur -> children;
  while(cur != NULL) {
    if (hasName(cur, "point"))
      if (getTripleFloat(cur, doc, &px, &py, &pz, "light: point"))
	return NULL;

    if (hasName(cur, "intensity"))
      if (getOneFloat(cur, doc, &intensity, "light: intensity"))
	return NULL;
    cur = cur -> next;
  }


#ifdef PARSEPRINT
  printf("light:\n");
  printf("point = %.2f %.2f %.2f\n", px, py, pz);
  printf("intensity = %.2f\n", intensity);
#endif
  return new Light(Point(px, py, pz), intensity);
}

/**
 * parsing the camera entry.
 */
Camera* parseCamera(xmlNodePtr cur, xmlDocPtr doc, 
		    uint32_t resx, uint32_t resy, uint32_t spp,
		    bool isPerspective) {
  float pos_x, pos_y, pos_z;
  float lookat_x, lookat_y, lookat_z;
  float up_x, up_y, up_z;
  float left, right, top, bottom;
  float near, far;

  Sampler *sampler = new StratifiedSampler(0, resx - 1, 0, resy - 1, spp);

  cur = cur -> children;
  while (cur != NULL) {
    if (hasName(cur, "point"))
      if (getTripleFloat(cur, doc, &pos_x, &pos_y, &pos_z, "camera : point"))
	return NULL;
    
    if (hasName(cur, "lookat"))
      if (getTripleFloat(cur, doc, &lookat_x, &lookat_y, &lookat_z, "camera : lookat"))
	return NULL;

    if (hasName(cur, "up"))
      if (getTripleFloat(cur, doc, &up_x, &up_y, &up_z, "camera : up"))
	return NULL;

    if (hasName(cur, "left"))
      if (getOneFloat(cur, doc, &left, "camera : left"))
	return NULL;

    if (hasName(cur, "right"))
      if (getOneFloat(cur, doc, &right, "camera : right"))
	return NULL;

    if (hasName(cur, "top"))
      if (getOneFloat(cur, doc, &top, "camera : top"))
	return NULL;

    if (hasName(cur, "bottom"))
      if (getOneFloat(cur, doc, &bottom, "camera : bottom"))
	return NULL;

    if (hasName(cur, "near"))
      if (getOneFloat(cur, doc, &near, "camera : near"))
	return NULL;

    if (hasName(cur, "far"))
      if (getOneFloat(cur, doc, &far, "camera : far"))
	return NULL;
    cur = cur -> next;
  }
  
#ifdef PARSEPRINT
  printf("camera:\n");
  printf("point = %.2f %.2f %.2f\n", pos_x, pos_y, pos_z);
  printf("up = %.2f %.2f %.2f\n", up_x, up_y, up_z);
  printf("lookat = %.2f %.2f %.2f\n", lookat_x, lookat_y, lookat_z);
  printf("left = %.2f right = %.2f\n", left, right);
  printf("top = %.2f bottom = %.2f\n", top, bottom);
  printf("near = %.2f\n far = %.2f\n", near, far);
#endif
  if (isPerspective)
    return new Perspective(Translation(pos_x, pos_y, pos_z),
			 Vector(lookat_x, lookat_y, lookat_z),
			 Vector(up_x, up_y, up_z),
			 sampler, left, right, top, bottom,
			 near, far);

  return new OrthoCamera(Translation(pos_x, pos_y, pos_z),
			 Vector(lookat_x, lookat_y, lookat_z),
			 Vector(up_x, up_y, up_z),
			 sampler, left, right, top, bottom,
			 near, far);
}

GMrender* parseScene(const char* file) {
  xmlDocPtr doc;
  xmlNodePtr root, cur;
  xmlChar *txt;
  uint32_t resx, resy, spp;
  bool isPerspective = false;
  int simd;
  GMrender *myrender;

  Object *tmpo;
  Camera *tmpc;
  Light  *tmpl;

  if ((doc = xmlParseFile(file)) == NULL) {
    fprintf(stderr, "The file is not valid.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  if ((root = xmlDocGetRootElement(doc)) == NULL) {
    fprintf(stderr, "This file doesn't contain a root element.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  if (xmlStrcmp(root -> name, (const xmlChar *) "scene")) {
    fprintf(stderr, "Missing Scene node.\n");
    xmlFreeDoc(doc);
    return NULL;
  }

  // get the property of resolution x
  if ((txt = xmlGetProp(root, (const xmlChar *)"resx")) == NULL) {
    fprintf(stderr, "Missing resolution x.\n");
    xmlFreeDoc(doc);
    return NULL;
  }

  resx = atoi((const char *)txt);
  xmlFree(txt);

  // get the property of resolution y
  if ((txt = xmlGetProp(root, (const xmlChar *)"resy")) == NULL) {
    fprintf(stderr, "Missing resolution y.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  resy = atoi((const char *)txt);
  xmlFree(txt);

  // get the property of rays_per_job
  if ((txt = xmlGetProp(root, (const xmlChar *)"rays_per_job")) == NULL) {
    fprintf(stderr, "Missing rays_per_job.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  Global::MAX_RAYS = atoi((const char *)txt);
  xmlFree(txt);

  // get the property of rays_per_job
  if ((txt = xmlGetProp(root, (const xmlChar *)"max_ray_round")) == NULL) {
    fprintf(stderr, "Missing max_ray_round.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  Global::MAX_LEVEL = atoi((const char *)txt);
  xmlFree(txt);

  if (resx * resy % Global::MAX_RAYS) {
    fprintf(stderr, "Error: the resolution should be multiply of %d.", Global::MAX_RAYS);
    return NULL;
  }

  if ((txt = xmlGetProp(root, (const xmlChar *)"spp")) == NULL) {
    fprintf(stderr, "Missing spp\n");
    xmlFreeDoc(doc);
    return NULL;
  }

  spp = atoi((const char*)txt);
  xmlFree(txt);

  // get the property of the number of SPU
  if ((txt = xmlGetProp(root, (const xmlChar *)"nspu")) == NULL) {
    fprintf(stderr, "Missing nspu.\n");
    xmlFreeDoc(doc);
    return NULL;
  }

  nspu = atoi((const char*)txt);
  xmlFree(txt);
  if (nspu > MAX_SPU || nspu <= 0) {
    fprintf(stderr, "Error: The number of SPU should be between 0 - %d.\n", MAX_SPU);
    return NULL;
  }

  if ((txt = xmlGetProp(root, (const xmlChar *)"camera")) == NULL) {
    fprintf(stderr, "Missing camera.\n");
    xmlFreeDoc(doc);
    return NULL;
  }
  
  if (strcmp((const char*)txt, "perspective") == 0) {
    isPerspective = true;
  }
  xmlFree(txt);
  
  if ((txt = xmlGetProp(root, (const xmlChar *)"simd")) == NULL) {
    fprintf(stderr, "Missing SIMD option.\n");
    xmlFreeDoc(doc);
    return NULL;
  }

  simd = atoi((const char*)txt);
  xmlFree(txt);

#ifdef PARSE_PRINT
  printf("Scene Summary:\n");
  printf("Resolution: %d x %d\n", resx, resy);
  printf("RAYS_PER_JOB: %d\n", Global::MAX_RAYS);
  printf("MAX_RAY_ROUND: %d\n", Global::MAX_LEVEL);
  printf("SAMPLE_PER_PIXEL: %d\n", spp);
#endif

  cur = root -> children;
  myrender = new GMrender();

  if (simd == 1) {
    myrender -> setAccelerator(new SimpleAccel(new VectorJobFactory()));
    myrender -> setIntegrator(new ReduceIntegrator(resx, resy));
  } else {
    myrender -> setAccelerator(new SimpleAccel(new SimpleJobFactory()));
    myrender -> setIntegrator(new WhittedIntegrator(resx, resy));
  }
  
  myrender -> setJobber(new CellJobber(nspu));

  while( cur != NULL ) {
    if (!xmlStrcmp(cur -> name, (const xmlChar *)"sphere")) {
      if ((tmpo = parseSphere(cur, doc)) != NULL)
	Global::global_add_object(tmpo);
    } else if (!xmlStrcmp(cur -> name, (const xmlChar *)"light")) {
      if ((tmpl = parseLight(cur, doc)) != NULL)
	Global::global_add_light(tmpl);
    } else if (!xmlStrcmp(cur -> name, (const xmlChar *)"camera")) {
      if ((tmpc = parseCamera(cur, doc, resx, resy, spp, isPerspective)) != NULL)
	myrender -> setCamera(tmpc);
    } else if (!xmlStrcmp(cur -> name, (const xmlChar *)"plane")) {
      if ((tmpo = parsePlane(cur, doc)) != NULL)
	Global::global_add_object(tmpo);
    }
    cur = cur -> next;
  }

  xmlFreeDoc(doc);
  return myrender;
}
