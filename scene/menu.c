#include <allegro5/allegro_primitives.h>
#include "menu.h"
#include "../Shapes/Point.h"
#include "../Shapes/Rectangle.h"
#include "../global.h"
/*
   [Menu function]
*/
Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 25, 0);
    pDerivedObj->img_b=al_load_bitmap("assets/image/menu_background.png");
    pDerivedObj->img[0][0]=al_load_bitmap("assets/image/start0.png");
    pDerivedObj->img[0][1]=al_load_bitmap("assets/image/start1.png");
    pDerivedObj->img[1][0]=al_load_bitmap("assets/image/exit0.png");
    pDerivedObj->img[1][1]=al_load_bitmap("assets/image/exit1.png");
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->start_state=false; // 0 on 1 off
    pDerivedObj->end_state=false;   // 0 on 1 off
    pDerivedObj->s_bx = 650;
    pDerivedObj->s_by = 478;
    pDerivedObj->e_bx = 650;
    pDerivedObj->e_by = 691;
    pDerivedObj->Title_x=WIDTH/2;
    pDerivedObj->Title_y=HEIGHT/2;
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    
    pDerivedObj->fptr=fopen("assets/id/data.txt","r");
    if(pDerivedObj->fptr == NULL)pDerivedObj->save=false;
    else pDerivedObj->save=true;
    fclose(pDerivedObj->fptr);

    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene *const pMenuObj)
{  
    Menu *Obj = ((Menu *)(pMenuObj->pDerivedObj));
    if(mouse_state[1] && mouse.x >= Obj->s_bx && mouse.x <= Obj->s_bx + 404 && 
    mouse.y >= Obj->s_by && mouse.y <= Obj->s_by + 151 ){
        Obj->start_state=true;
    }
    if(mouse.x >= Obj->e_bx&& mouse.x <= Obj->e_bx + 404 
    && mouse.y >= Obj->e_by && mouse.y <= Obj->e_by + 151 && mouse_state[1]){
        Obj->end_state=true;
    }
    if(!mouse_state[1] && Obj->start_state){
        if(Obj->save){
            if(key_state[ALLEGRO_KEY_ENTER]){
                Obj->fptr=fopen("assets/id/data.txt","r");
                if(Obj->fptr!=NULL){
                    int value;  char tmp[1000];
                    for(int i=1;i<=8;i++){
                        fscanf(Obj->fptr,"%s %d\n", tmp, &value);
                        player_skill_level[i]=value;
                        printf("%d\n",value);
                    }
                    printf("successfully save file\n");
                }
                else
                    perror("Fails message:");
                pMenuObj->scene_end=true;
                window=1;
            }
            else if(key_state[ALLEGRO_KEY_SPACE]){
                pMenuObj->scene_end=true;
                window=1;
            }    
        }
        else{
            pMenuObj->scene_end=true;
            window=1;
        }
    } 
    else if(!mouse_state[1] && Obj->end_state){
            pMenuObj->scene_end=true;
            window=-1;
        }
    }
void menu_draw(Scene *const pMenuObj)
{
    Menu *Obj = ((Menu *)(pMenuObj->pDerivedObj));
    if(Obj->save && !mouse_state[1] && Obj->start_state){
        char tmp[100]="Detected previous game save data. Press Enter to load, or press Space to skip.";
        al_clear_to_color(al_map_rgb(0,0,0));
        al_draw_text(Obj->font,al_map_rgb(255,255,255),Obj->Title_x,Obj->Title_y,ALLEGRO_ALIGN_CENTRE, tmp);
    }
    else{
        al_play_sample_instance(Obj->sample_instance);
        al_draw_bitmap(Obj->img_b,0,0,0);
        al_draw_bitmap(Obj->img[0][Obj->start_state],Obj->s_bx,Obj->s_by,0);
        al_draw_bitmap(Obj->img[1][Obj->end_state],Obj->e_bx,Obj->e_by,0);
    }
}
void menu_destroy(Scene *const pMenuObj)
{
    Menu *Obj = ((Menu *)(pMenuObj->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    al_destroy_bitmap(Obj->img_b);
    for(int i=0;i<2;i++){
        for(int j=0;j<2;j++){
            al_destroy_bitmap(Obj->img[i][j]);
        }
    }
    free(Obj);
    free(pMenuObj);
}
