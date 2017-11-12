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
object_t *create_object(SDL_Surface *surface, triangle_t *inputModel, int numtriangles)
{
    //1. Allocate memory
    //2. Fill memory
    //3. Return memory

    object_t *obj = malloc(sizeof(object_t));
    obj->scale = randomNum(0.04, 0.12);
    obj->rotation = 1;
    obj->tx = randomNum(100, 1500);
    obj->ty = randomNum(30, 200);
    obj->speedx = randomNum(2.0, 14.0);
    obj->speedy = randomNum(1, 5.5);
    if (randomNum(0.0, 1.0) >= 0.5) 
        obj->speedx *= -1;
    
    //ttl * 60 because 60 FPS
    obj->ttl = 5 * 60;
    obj->fillcolor = (int)randomNum(100, RAND_MAX);
    
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
        obj->model[i].fillcolor = obj->fillcolor; 
        //Change colors halfway through the array. 
        if (i > obj->numtriangles / 2 + 15 ) //+15 to get an even split of the ball
        obj->model[i].fillcolor *= 6;
        
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
        if (i != 0){
            //make sure model[0] doesn't get changed twice.
            obj->model[i].tx = obj->model->tx - obj->speedx;
            obj->model[i].ty = obj->model->ty - obj->speedy;
            
        }
        obj->model[i].rotation += (obj->rotation * obj->speedx);
        
        obj->model[i].tx += obj->speedx;
        obj->model[i].ty += obj->speedy;
        draw_triangle(obj->surface, &obj->model[i]);
    }
} 

float randomNum(float min, float max) {
    
    float num = ((float)rand()/(float)(RAND_MAX)) * max;
    if (num < min) 
        num += min;

    return num;
}