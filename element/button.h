#ifndef Button_H_INCLUDED
#define Button_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include <allegro5/allegro_image.h>
#include "../Shapes/Rectangle.h"
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
/*
   [Button object]
*/

typedef struct _Button
{   ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    ALLEGRO_BITMAP *img[2];
    double cx, cy;
    int width,height;
    bool press;
} Button;
Elements *New_Button(int label,double x, double y);
void Button_update(Elements *const ele);
void Button_interact(Elements *const, Elements *const);
void Button_draw(Elements *const ele);
void Button_destory(Elements *const ele);

#endif
