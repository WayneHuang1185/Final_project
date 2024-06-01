#ifndef GameEnd_H_INCLUDED
#define GameEnd_H_INCLUDED
#include "scene.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
/*
   [GameEnd object]
*/
typedef struct _GameEnd
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song[2];
    ALLEGRO_BITMAP *img[2];
    ALLEGRO_SAMPLE_INSTANCE *sample_instance[2];
    int title_x, title_y;
    int id;
} GameEnd;
Scene *New_GameEnd(int label,int id);
void default_song_and_paly(Scene *const pGameEndObj);
void GameEnd_update(Scene *const pGameEndObj);
void GameEnd_draw(Scene *const pGameEndObj);
void GameEnd_destroy(Scene *const pGameEndObj);

#endif
