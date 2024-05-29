#include"monster.h"
#include"bullet.h"
#include"player.h"
#include"../global.h"
#include"../shapes/Rectangle.h"
#include "../shapes/Shape.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
/*
速度型怪物: id:0 
           hp:50 
           move_speed:5
           damage:30
           hp_recovery:5
           atk_frequency:50
           recover:200
           exp:300
           
坦克怪物 :  id:1 
           hp:500
           move_speed:1
           damage:50
           hp_recovery:20
           atk_frequency:200
           recover:300
           exp:500

中型怪物 :  id:2 
           hp:250
           move_speed:3
           damage:40
           hp_recovery:20
           atk_frequency:150
           recover:250
           exp:600
           
Boss :     id:3
           hp:2000
           move_speed:6
           damage:100
           hp_recovery:100
           atk_frequency:150
           recover:250
           exp: 1500  
特性:繳械(若玩家無法逃出怪物攻擊範圍，其將無法攻擊)
    爆炸(若使其接觸玩家時間過長，怪物將自爆，並將造成高額爆炸傷害)
    嗜血(對玩家造成)
*/

Elements *New_Monster(int label){
    Monster *pDerivedObj = (Monster *)malloc(sizeof(Monster));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = 200;
    pDerivedObj->y = 200;
    pDerivedObj->move_speed=2;
    pDerivedObj->exp=1000;
    pDerivedObj->hp=10;
    pDerivedObj->hp_max=10;
    pDerivedObj->hp_recovery=5;
    pDerivedObj->recovery=200;
    pDerivedObj->atk_timer=0;
    pDerivedObj->atk_frequency=200;
    pDerivedObj->damage=60;
    pDerivedObj->img = al_load_bitmap("assets/image/monster.png");
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x-pDerivedObj->width/2,
                                        pDerivedObj->y-pDerivedObj->height/2,
                                        pDerivedObj->x+pDerivedObj->width/2,
                                        pDerivedObj->y+pDerivedObj->height/2);
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
void Monster_update(Elements *const self)
{   
    Monster *Obj = ((Monster*)(self->pDerivedObj));
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
            double angle=atan2(pl->hitbox->center_y(pl->hitbox) - mon->hitbox->center_y(mon->hitbox), pl->hitbox->center_x(pl->hitbox) - mon->hitbox->center_x(mon->hitbox));
            int dx=mon->move_speed*cos(angle),dy=mon->move_speed*sin(angle);
            _Monster_update_postion(self,dx,dy);
        }
        else{
            
        }
    }
    if(ele->label == Bullet_L){
       Bullet *bu=((Bullet*)(ele->pDerivedObj));
        if(mon->hitbox->overlap(mon->hitbox,bu->hitbox)){
            mon->hp-=bu->damage;
            ele->dele=true;
            mon->recovery=200;
        }       
    }   
}
void Monster_draw(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    Obj->hitbox->draw_hitbox(Obj->hitbox);
    al_draw_bitmap(Obj->img,Obj->x-Obj->width/2, Obj->y-Obj->height/2,0);

}
void Monster_destory(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    free(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}