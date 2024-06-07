#include"./gamescene.h"
#include"../element/Ball.h"
#include"../element/player.h"
#include"../element/bullet.h"
#include"../element/monster.h"
#include"../element/playerManager.h"
#include"../global.h"
/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/gamescene.png");
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 40, 0);
    pDerivedObj->Title_x=WIDTH/2;
    pDerivedObj->Title_y=HEIGHT/2;
    pObj->pDerivedObj = pDerivedObj;
    // register element
    /*
    _Register_elements(pObj, New_Floor(Floor_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Character(Character_L));
    */
    //_Register_elements(pObj, New_Ball(Ball_L));
    _Register_elements(pObj, New_Player(Player_L));
    _Register_elements(pObj, New_player_Manager(PlayerManager_L));
    //_Register_elements(pObj,New_Monster(Monster_L,0));
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}

void game_scene_update(Scene *const pGameSceneObj)
{
    // update every element
    ElementVec allEle = _Get_all_elements(pGameSceneObj);
    for (int i = 0; i < allEle.len; i++)
        allEle.arr[i]->Update(allEle.arr[i]);
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        // run every interact object
        for (int j = 0; j < ele->inter_len; j++)
        {
            int inter_label = ele->inter_obj[j];
            ElementVec labelEle = _Get_label_elements(pGameSceneObj, inter_label);
            for (int i = 0; i < labelEle.len; i++)
                ele->Interact(ele, labelEle.arr[i]);
        }
    }
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(pGameSceneObj, ele);
    }
    for(int i = 0; i < allEle.len; i++){  
        if(allEle.arr[i]->label == PlayerManager_L){
           // printf("ok\n");
            PlayerManager*pm=(PlayerManager*)(allEle.arr[i]->pDerivedObj);
            if(pm->total_monster<=0 && pm->update_stage == false){
                if(key_state[ALLEGRO_KEY_ENTER]){
                    FILE *fptr=fopen("assets/id/data.txt","w");
                    if(fptr!=NULL){
                        char tmp[50];
                        for(int i=1;i<=8;i++){
                            printf("%d ",player_skill_level[i]);
                            sprintf(tmp,"%s %d",skill[i],player_skill_level[i]);
                            fprintf(fptr,"%s\n",tmp);
                        }
                        printf("\n");
                        printf("successfully save file\n");
                    }
                    else
                        perror("Fails message");
                    fclose(fptr);
                    pm->update_stage=true;
                    pm->save=true;
                }
                else if(key_state[ALLEGRO_KEY_SPACE]){
                    pm->update_stage=true;  
                    pm->save=true;
                }
            }
        }
    }
}
void game_scene_draw(Scene *const pGameSceneObj)
{   
    GameScene *gs = ((GameScene *)(pGameSceneObj->pDerivedObj));
    ElementVec pmEle= _Get_label_elements(pGameSceneObj,PlayerManager_L);
    PlayerManager*pm=(PlayerManager*)(pmEle.arr[0]->pDerivedObj);
    if(pm->total_monster<=0 && pm->update_stage == false){
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_text(gs->font,al_map_rgb(255,255,255),gs->Title_x,gs->Title_y,ALLEGRO_ALIGN_CENTRE,"Press Enter to save, or Space to skip.");
    }
    else{
        ElementVec allEle = _Get_all_elements(pGameSceneObj);
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_bitmap(gs->background, 0, 0, 0);
        for (int i = 0; i < allEle.len; i++)
        {
            Elements *ele = allEle.arr[i];
            ele->Draw(ele);
        }
    }
}

void game_scene_destroy(Scene *const pGameSceneObj)
{
    GameScene *Obj = ((GameScene *)(pGameSceneObj->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    ElementVec allEle = _Get_all_elements(pGameSceneObj);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(pGameSceneObj);
}
