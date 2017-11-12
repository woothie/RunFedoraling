#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include "drawline.h"
#include "triangle.h"
#include "object.h"

#define MAX(x,y)    (x > y ? x : y)
#define MAX3(x,y,z) MAX(MAX(x,y), MAX(x,z))

float randomNum(float min, float max); 
/*
 * Return a newly created object based on the arguments provided.
 */
object_t *create_object(SDL_Surface *surface, triangle_t *inputModel, int numtriangles,int tx,int ty, int speed)
{
    //1. Allocate memory
    //2. Fill memory
    //3. Return memory

    object_t *obj = malloc(sizeof(object_t));
    obj->scale = inputModel->scale;
    obj->rotation = 0;
    obj->tx = tx;
    obj->ty = ty;
    obj->speedx = 0;
    obj->speedy = speed;
    
    obj->fillcolor = inputModel->fillcolor;
    
    obj->numtriangles = numtriangles;
    obj->model = malloc(sizeof(triangle_t) * numtriangles);
    memcpy(obj->model, inputModel, sizeof(triangle_t) * numtriangles);

    obj->surface = surface;
    
    int maxX = 0;
    //Overwrite model array variables with obj variables
    for (int i = 0; i < obj->numtriangles; i++) {
        obj->model[i].tx += obj->tx;
        obj->model[i].ty += obj->ty;
        obj->model[i].scale = obj->scale;
        obj->model[i].fillcolor = inputModel[i].fillcolor;
        obj->model[i].rotation = obj->rotation;
        
        if (MAX3(obj->model[i].x1, obj->model[i].x2, obj->model[i].x3) > maxX)
            maxX = MAX3(obj->model[i].x1, obj->model[i].x2, obj->model[i].x3);
    }
    obj->radius = (int)((float)maxX*obj->scale);

    return obj;
}

/*
 * Destroy the object, freeing the memory.
 */
void destroy_object(object_t *obj)
{
    free(obj->model);
    free(obj);
}

/*
 * Draw the object on its surface.
 */
void draw_object(object_t *obj)
{
    for (int i = 0; i < obj->numtriangles; i++) {
        //make sure model[0] doesn't get changed twice.
        obj->model[i].tx = obj->model->tx - obj->speedx;
        obj->model[i].ty = obj->model->ty - obj->speedy;
        obj->model[i].rotation += 1;
        
        obj->model[i].tx += obj->speedx;
        obj->model[i].ty += obj->speedy;
        rotate_triangle(&obj->model[i]);
        draw_triangle(obj->surface, &obj->model[i]);
    }
} 

float randomNum(float min, float max) {
    
    float num = ((float)rand()/(float)(RAND_MAX)) * max;
    if (num < min) 
        num += min;

    return num;
}