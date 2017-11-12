#ifndef OBJECT_H_
#define OBJECT_H_

#include "triangle.h"
#include <SDL.h>

typedef struct object object_t;

struct object {
    float       scale;          /* Object scale */
    float       rotation;       /* Object rotation */
    float       tx, ty;         /* Position on screen */
    int radius;         /* radius for ball type object*/
    
    int         fillcolor;      /* color of ball*/
    float       speedx, speedy; /* Object speed in x and y direction */
    unsigned int ttl;           /* Time till object should be removed from screen */
    
    int         numtriangles;   /* Number of triangles in model */
    triangle_t  *model;         /* Model triangle array */

    SDL_Surface *surface;       /* SDL screen */
};


/*
 * Return a newly created object based on the arguments provided.
 */
object_t *create_object(SDL_Surface *surface, triangle_t *triangles, int numtriangles, int tx,int ty, int speed);
    

/*
 * Destroy the object, freeing the memory.
 */
void destroy_object(object_t *object);  

/*
 * Draw the object on its surface.
 */
void draw_object(object_t *object);

#endif /*OBJECT_H_*/
