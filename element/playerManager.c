#include "playerManager.h"
#include "../scene/sceneManager.h"
#include<time.h>
/*
   [player_Manager function]
*/


Elements *New_player_Manager(int label)
{   PlayerManager *pDerivedObj = (PlayerManager*)malloc(sizeof(PlayerManager));
    Elements *Obj = New_Elements(label);
    srand(time(NULL));
    pDerivedObj->update_stage=true;
    pDerivedObj->total_monster=0;
    pDerivedObj->stage=0;
    pDerivedObj->pl_x=pDerivedObj->pl_y=0;
    pDerivedObj->store_exp=0;
    Obj->pDerivedObj= pDerivedObj;
    Obj->inter_obj[Obj->inter_len++] = Player_L;
    Obj->inter_obj[Obj->inter_len++] = Monster_L;
    Obj->Draw = playerManager_draw;
    Obj->Update = playerManager_update;
    Obj->Interact = playerManager_interact;
    Obj->Destroy = playerManager_destory;
    return Obj;
}

void playerManager_draw(Elements *const ele)
{
}
void playerManager_destory(Elements *const ele)
{
  
}
/*-------------------------------------------------------
    bulletDamage,
    bulletRecovery,
    hpRecovery,
    mpRecovery,
    hpMax,
    mpMax,
    expMax*/
/*
stage1:                 stage2:             stage3:              stage4:              stage5:   
id: 0   1   2   3       id: 0   1   2   3   id: 0   1   2   3    id: 0   1   2   3    id: 0   1   2   3  
num: 1   0   0   0     num: 1   1   0   0  num: 1   1   1   0   num: 2   2   3   0   num: 0   0   0   1                            
*/

const int player_save_area=100;
const int monster_produce_area=800;
const int MAXIMUM_STAGE=5;
const int MAXIMUM_MONSTER=10;

typedef struct{
  double x, y;
}Location;

int stage[5][4][1]={
    {{1},{2},{0},{0}},
    {{10},{20},{10},{0}},
    {{10},{30},{20},{0}},
    {{20},{20},{20},{0}},
    {{30},{20},{10},{2}}  
};
void playerManager_update(Elements *const ele)
{   
    PlayerManager*pm=(PlayerManager*)(ele->pDerivedObj);
    printf("%d\n",window);
    if(pm->update_stage){
        int num;
        for(int i=0;i<4;i++){
            num=stage[pm->stage][i][0];
            pm->total_monster+=num;
            for(int j=0;j<num;j++){
                double sx,sy;
                sx=(int)(rand()%monster_produce_area+player_save_area+pm->pl_x)%(int)WIDTH,sy=(int)(rand()%monster_produce_area+player_save_area+pm->pl_y)%(int)(HEIGHT);
                _Register_elements(scene, New_Monster(Monster_L,i,sx,sy));
            }
        }
        pm->update_stage=false;
    }
    if(pm->total_monster <= 0){
        if(pm->stage<MAXIMUM_STAGE-1){
            pm->update_stage=true;
            pm->stage+=1;
        }
        else{
            scene->scene_end=true;
            window=2;
        }
    }
    
    
}
void playerManager_interact(Elements *const self, Elements *const target) {
    int skill_data[10][8]={ 
        {5,4,3,3,2,2,2,2},//spGain
        {2,3,4,5,6,7,8,9},//moveSpeed
        {2,3,4,5,6,7,8,10},//bulletSpeed
        {30,40,50,60,70,80,90,100},//bulletDamage
        {300,230,170,120,80,50,25,10},//bulletReload
        {5,10,15,20,25,30,35,40},//hpRecovery
        {5,10,15,20,25,30,40,50},//mpRecovery
        {300,400,500,600,700,800,900,1000},//hpMax
        {150,300,450,600,800,1000,1200,1600},//mpMax
        {300,600,1000,1400,1800,2300,2800,3200}//expMax
    };
    PlayerManager *pm=((PlayerManager *)(self->pDerivedObj));
    if(target->label == Player_L){
        Player *pl=((Player*)(target->pDerivedObj));
        pm->pl_x=pl->x,pm->pl_y=pl->y;
        if(pl->update_change){
            pl->move_speed=skill_data[moveSpeed][pl->skill_level[moveSpeed]];
            pl->bullet_speed=skill_data[bulletSpeed][pl->skill_level[bulletSpeed]];
            pl->bullet_damage=skill_data[bulletDamage][pl->skill_level[bulletDamage]];
            pl->bullet_reload=skill_data[bulletReload][pl->skill_level[bulletReload]];
            pl->hp_recovery=skill_data[hpRecovery][pl->skill_level[hpRecovery]];
            pl->mp_recovery=skill_data[mpRecovery][pl->skill_level[mpRecovery]];
            pl->hp_max=skill_data[hpMax][pl->skill_level[hpMax]];
            pl->mp_max=skill_data[mpMax][pl->skill_level[mpMax]];
            pl->update_change=false;
        }
        if(pm->store_exp>0){
            pl->exp+=pm->store_exp;
            pm->store_exp=0;
        }
        if(pl->skill_level[expMax]<10 && pl->exp > skill_data[expMax][pl->skill_level[expMax]]){
            pl->level+=1;
            pl->sp+=skill_data[spGain][pl->skill_level[spGain]];
            pl->skill_level[expMax]+=1;
            pl->skill_level[spGain]+=1;
        }
        
    } 
    if(target->label == Monster_L){
        Monster *mon=((Monster*)(target->pDerivedObj));
        if(mon->hp<=0){
            target->dele=true;
            pm->store_exp+=mon->exp;
            pm->total_monster-=1;
        }
    }
}


