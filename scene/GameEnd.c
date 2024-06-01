#include <allegro5/allegro_primitives.h>
#include "GameEnd.h"
#include "../Shapes/Point.h"
#include "../Shapes/Rectangle.h"
#include "../global.h"
/*
   [GameEndfunction]
*/
Scene *New_GameEnd(int label,int id)
{ 
    GameEnd*pDerivedObj = (GameEnd*)malloc(sizeof(GameEnd));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    /*
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    pDerivedObj->song[0] = al_load_sample("assets/sound/gameend_v.mp3");
    pDerivedObj->song[1] = al_load_sample("assets/sound/gameend_d.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance[0] = al_create_sample_instance(pDerivedObj->song[0]);
    pDerivedObj->sample_instance[1] = al_create_sample_instance(pDerivedObj->song[1]);*/
    pDerivedObj->img[0]=al_load_bitmap("assets/image/victory_t.png");
    pDerivedObj->img[1]=al_load_bitmap("assets/image/defeat_t.png");
    pDerivedObj->id=id;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = GameEnd_update;
    pObj->Draw = GameEnd_draw;
    pObj->Destroy = GameEnd_destroy;
    
    return pObj;
}
void default_song_and_paly(Scene *const pGameEndObj){
     GameEnd *Obj = ((GameEnd *)(pGameEndObj->pDerivedObj));
     al_set_sample_instance_playmode(Obj->sample_instance[Obj->id], ALLEGRO_PLAYMODE_LOOP);
     al_restore_default_mixer();
     al_attach_sample_instance_to_mixer(Obj->sample_instance[Obj->id], al_get_default_mixer());
     al_play_sample_instance(Obj->sample_instance[Obj->id]);
}
void GameEnd_update(Scene *const pGameEndObj)
{
    pGameEndObj->scene_end=false;
}
void GameEnd_draw(Scene *const pGameEndObj)
{
    GameEnd*Obj = ((GameEnd*)(pGameEndObj->pDerivedObj));
    al_draw_bitmap(Obj->img[Obj->id],-150,0,0);
    //default_song_and_paly(pGameEndObj);
}
void GameEnd_destroy(Scene *const pGameEndObj)
{
    GameEnd*Obj = ((GameEnd*)(pGameEndObj->pDerivedObj));
    al_destroy_font(Obj->font);
    for(int i=0;i<2;i++){
       // al_destroy_sample(Obj->song[i]);
        //al_destroy_sample_instance(Obj->sample_instance[i]);
        al_destroy_bitmap(Obj->img[i]);
    }
    free(Obj);
    free(pGameEndObj);
}
