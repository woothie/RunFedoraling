#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>
#include "drawline.h"
#include "triangle.h"
#include "list.h"
#include "teapot_data.h"
#include "sphere_data.h"
#include "deathBall_data.h"
#include "object.h"
#include "time.h"

/* Two macro's that find the lesser or greater of two values */
#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

/* screen width height and FPS*/
const int screen_w = 1600;
const int screen_h = 900;
const int white = 0xeeeeee;
#define TICK_INTERVAL 1000 / 60;
static unsigned int next_time;

void death_Ball(object_t* deathBall);

/*
 * Clear the surface by filling it with 0x00000000(black).
 */
void clear_screen(SDL_Surface *surface)
{
    if(SDL_FillRect(surface, NULL, 0x00000000) < 0){
        fprintf(stderr, "Unable to clear the surface. Error returned: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

/*
 * Accelerate the object; altering its speed based on the boost given.
 */
void accelerate_object(object_t *a, float boost, float maxspeed)
{
    float s;
    float news;
    
    /* Calculate lenght of speed vector */
    s = sqrtf(a->speedx * a->speedx + a->speedy * a->speedy);

    /* Boost speed */
    news = s * boost;
    if (news < 0.0)
        news = 0.0;
    if (news > maxspeed)
        news = maxspeed;    
    a->speedx *= news/s;
    a->speedy *= news/s;
}

/*
 * Limit fps
*/
unsigned int time_left(void)
{
    unsigned int now;

    now = SDL_GetTicks();
    if (next_time <= now)
        return 0;
    else 
        return next_time - now;
}

/*
 * Animate bouncing balls on the screen.
 */
void bouncing_balls(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Surface *surface)
{   

    srand(time(NULL));
    SDL_Event event;
    next_time = SDL_GetTicks() + TICK_INTERVAL;

    object_t *deathBall = create_object(surface, deathBall_model, DEATHBALL_NUMTRIANGLES, 210, 500, 0);

    while(1) {
        clear_screen(surface);
        draw_line(surface, 1, 300, screen_w - 1, 300, white);
        draw_line(surface, 1, screen_h - 100, screen_w-1, screen_h-100, white);


        death_Ball(deathBall);





        /* Update the texture, it to the video buffer and present it */
        SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch); 
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        /* Detect that the user tries to quit the application */
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT)
            break;

        /* framerate cap */
        SDL_Delay(time_left()); 
        next_time += TICK_INTERVAL; 
    }
}

void death_Ball(object_t* deathBall) {

    draw_object(deathBall);
}

int main(void)
{
    const size_t bufsize = 100;


    char errmsg[bufsize];
    SDL_Surface *surface;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *texture;
    
    /* Initialize SDL */
    if (SDL_Init(SDL_INIT_VIDEO) < -1) {
        snprintf(errmsg, bufsize, "Unable to initialize SDL.");
        goto error;
    }
    
    /* Create a 1600x900 window */
    window = SDL_CreateWindow("The Amazing Bouncing Balls",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              screen_w, screen_h,
                              0);
    if(!window) {
        snprintf(errmsg, bufsize, "Unable to get video surface.");
        goto error;
    }

    /* Create window renderer */
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer) {
        snprintf(errmsg, bufsize, "Unable to create window renderer.");
        goto error;
    }

    /* Create the texture that contain the currentrly drawn pixels */
    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STATIC,
                                screen_w, screen_h);
    if(!texture) {
        snprintf(errmsg, bufsize, "Unable to create texture.");
        goto error;
    }

    /* Create the suface in RAM that we manipulate the pixels of */
    surface = SDL_CreateRGBSurface(0, screen_w, screen_h, 32,
                                   0x00FF0000,
                                   0x0000FF00,
                                   0x000000FF,
                                   0xFF000000);
    if(!surface) {
        snprintf(errmsg, bufsize, "Unable to create surface.");
        goto error;
    }

    /* Start bouncing some balls */
    bouncing_balls(renderer, texture, surface);

    /* Shut down SDL */
    SDL_Quit();

    /* Wait a little bit jic something went wrong (so that printfs can be read) */
    SDL_Delay(5000);
    
    return 0;

    /* Upon an error, print message and quit properly */
error:
    fprintf(stderr, "%s Error returned: %s\n", errmsg, SDL_GetError());
    SDL_Quit();
    exit(EXIT_FAILURE);
}
