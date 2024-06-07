#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include "scene.h"
#include"../Shapes/Shape.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../global.h"
/*
   [Menu object]
*/
typedef struct _Menu
{   FILE *fptr;
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_BITMAP *img[2][2];
    ALLEGRO_BITMAP *img_b;
    Shape *hitbox;
    bool start_state,end_state,save;
    int s_bx,s_by,e_bx,e_by;
    double Title_x,Title_y;
} Menu;
Scene *New_Menu(int label);
void menu_update(Scene *const pMenuObj);
void menu_draw(Scene *const pMenuObj);
void menu_destroy(Scene *const pMenuObj);

#endif
