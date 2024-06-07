#include"monster.h"
#include"monster_property.h"
#include"bullet.h"
#include"player.h"
#include"../global.h"
#include"../shapes/Rectangle.h"
#include "../shapes/Shape.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
/*----------------------------------------------------------------------------------------------------------------
速度型怪物: id:0, hp:50 , hp_recovery:5, move_speed:3.5, damage:0, atk_frequency:50, recover:200, exp:300
------------------------------------------------------------------------------------------------------------------
坦克怪物 :  id:1, hp:500, hp_recovery:20, move_speed:1, damage:50, atk_frequency:200, recover:300, exp:500
------------------------------------------------------------------------------------------------------------------
中型怪物 :  id:2, hp:250, hp_recovery:20, move_speed:3, damage:40, atk_frequency:150, recover:250, exp:600
------------------------------------------------------------------------------------------------------------------
Boss :     id:3, hp:2000, hp_recovery:100, move_speed:6, damage:100, atk_frequency:150, recover:250, exp: 1500
------------------------------------------------------------------------------------------------------------------*/

double basicInfo[4][7]={
    {50  , 5  , 5, 30 , 50 , 200, 300},
    {500 , 20 , 1, 50 , 200, 300, 500},
    {250 , 20 , 3, 40 , 150, 250, 600},
    {2000, 100, 6, 100, 150, 250, 1500}
};
const int mon_scaled=70;
Elements *New_Monster(int label, int id, double x, double y){

    // setting derived object member
    Monster *pDerivedObj = (Monster *)malloc(sizeof(Monster));
    Elements *pObj = New_Elements(label);
    
    // geometry information setting
    char tmp[50];
    sprintf(tmp, "assets/image/monster%d_nohurt.png", id);
    pDerivedObj->img[0] = al_load_bitmap(tmp);
    sprintf(tmp, "assets/image/monster%d_hurt.png", id);
    pDerivedObj->img[1] = al_load_bitmap(tmp);
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img[0]);
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img[0]);
    
    // random position and hitbox setting
    // pDerivedObj->x = rand()%300+100, pDerivedObj->y = rand()%300+100;
    pDerivedObj->id=id;
    pDerivedObj->x = x;
    pDerivedObj->y = y;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x-mon_scaled/2, pDerivedObj->y-mon_scaled/2,
                                        pDerivedObj->x+mon_scaled/2, pDerivedObj->y+mon_scaled/2);
    
    // basic infomation setting
    pDerivedObj->hp=pDerivedObj->hp_max=basicInfo[id][0];
    pDerivedObj->hp_recovery=basicInfo[id][1];
    pDerivedObj->move_speed=basicInfo[id][2];
    pDerivedObj->damage=basicInfo[id][3];
    pDerivedObj->atk_frequency=basicInfo[id][4];
    pDerivedObj->recovery=basicInfo[id][5];
    pDerivedObj->exp=basicInfo[id][6];
    pDerivedObj->touch=false;
    // intitialise timers
    pDerivedObj->hurt=false;
    pDerivedObj->atk_timer=0;
    pDerivedObj->hp_timer=0;
    pDerivedObj->hurt_timer=0;
    pDerivedObj->exploded_timer=180;
    //pDerivedObj->property=New_Monster_Property(id);
    // add interact objects
    pObj->inter_obj[pObj->inter_len++] = Bullet_L;
    pObj->inter_obj[pObj->inter_len++] = Player_L;
    

    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Monster_update;
    pObj->Interact = Monster_interact;
    pObj->Draw = Monster_draw;
    pObj->Destroy = Monster_destory;
    return pObj;
    
}
void _Monster_update_postion(Elements *const self,double dx, double dy){
    Monster *mon= ((Monster *)(self->pDerivedObj));
    mon->x += dx;
    mon->y += dy;
    Shape *hitbox = mon->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}
void _Monster_super_power(Elements *const self){
    Monster *mon= ((Monster *)(self->pDerivedObj));
    int id=mon->id;
    switch(id){
        case 0:
           
        case 1:
         
        case 2:
            break;
        case 3:
            break;
    }
}
void Monster_update(Elements *const self)
{   
    Monster *Obj = ((Monster*)(self->pDerivedObj));
    Obj->hurt_timer=Obj->hurt_timer%32+1;
    Obj->recovery-=5;
    Obj->atk_timer+=5;
    if(Obj->recovery == 0){
        Obj->hp+=Obj->hp_recovery;
        Obj->hp=(Obj->hp > Obj->hp_max)? Obj->hp_max : Obj->hp;
        Obj->recovery=200;
    }
}
void Monster_interact(Elements *const self, Elements *const ele){
    Monster *mon = ((Monster *)(self->pDerivedObj));
    if(ele->label == Player_L){
        Player *pl=((Player*)(ele->pDerivedObj));
        if((mon->x - pl->x)*(mon->x - pl->x) + (mon->y - pl->y)*(mon->y - pl->y) > 1600){
            mon->touch=true;
            double r=0.3;
            double angle=atan2(pl->hitbox->center_y(pl->hitbox) - mon->hitbox->center_y(mon->hitbox), pl->hitbox->center_x(pl->hitbox) - mon->hitbox->center_x(mon->hitbox))+
            (double)(rand() % 200 - 100) / 50 * r;;
            double dx=mon->move_speed*cos(angle),dy=mon->move_speed*sin(angle);
            _Monster_update_postion(self,dx,dy);
        }
        else{
            mon->touch=false;
        }
        if(mon->hitbox->overlap(mon->hitbox, pl->hitbox) && mon->hurt_timer % 31 == 0){pl->hurt = mon->hurt = true;mon->hurt_timer=1;}
        else if(mon->hurt_timer % 16 == 0) pl->hurt = mon->hurt = false;
    }
    if(ele->label == Bullet_L){
       Bullet *bu=((Bullet*)(ele->pDerivedObj));
        if(mon->hitbox->overlap(mon->hitbox,bu->hitbox)){
            mon->hp-=bu->damage;
            ele->dele=true;
            mon->recovery=200;
        }       
        if(mon->hitbox->overlap(mon->hitbox, bu->hitbox) && mon->hurt_timer % 31 == 0){mon->hurt = true;mon->hurt_timer=1;}
        else if(mon->hurt_timer % 16 == 0) mon->hurt = false;
    }   
}
void Monster_draw(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    Obj->hitbox->draw_hitbox(Obj->hitbox);
    al_draw_scaled_bitmap(Obj->img[Obj->hurt],0, 0,
    Obj->width,Obj->height,Obj->x-mon_scaled/2,Obj->y-mon_scaled/2,mon_scaled,mon_scaled,0);

}
void Monster_destory(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    for(int i=0;i<2;i++)
        al_destroy_bitmap(Obj->img[i]);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}