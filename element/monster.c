#include"monster.h"
#include"bullet.h"
#include"player.h"
#include"../global.h"
#include"../shapes/Circle.h"
#include "../shapes/Shape.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
Elements *New_Monster(int label){
    Monster *pDerivedObj = (Monster *)malloc(sizeof(Monster));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = 30;
    pDerivedObj->y = 30;
    pDerivedObj->move_speed=2;
    pDerivedObj->hp=1000;
    pDerivedObj->hp_max=1000;
    pDerivedObj->atk_timer=0;
    pDerivedObj->hp_recovery=5;
    pDerivedObj->hp_timer=0;
    pDerivedObj->recovery=200;
    pDerivedObj->atk_frequency=200;
    pDerivedObj->damage=40;
    pDerivedObj->img = al_load_bitmap("assets/image/monster.png");
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
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
void _Monster_update_postion(Elements *const self,int dx, int dy){
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
    printf("%d\n",Obj->hp);
    Obj->hp_timer+=5;
    Obj->atk_timer+=5;
    if(Obj->hp_timer >= Obj->recovery){
        Obj->hp+=Obj->hp_recovery;
        Obj->hp=(Obj->hp > Obj->hp_max)? Obj->hp_max : Obj->hp;
        Obj->hp_timer%= Obj->recovery;
    }

}
void Monster_interact(Elements *const self, Elements *const ele){
    Monster *mon = ((Monster *)(self->pDerivedObj));
    if(ele->label == Player_L){
       Player*pl=((Player*)(ele->pDerivedObj));
       double angle=atan2(pl->y-mon->y,pl->x-mon->x);
       int dx=mon->move_speed*cos(angle),dy=mon->move_speed*sin(angle);
       _Monster_update_postion(self,dx,dy);
    }
    if(ele->label == Bullet_L){
       Bullet *bu=((Bullet*)(ele->pDerivedObj));
       if(mon->hitbox->overlap(mon->hitbox,bu->hitbox)){
           if(mon->hp>bu->damage){
              mon->hp-=bu->damage;
              ele->dele=true;
           }
           else{
              self->dele=true;
           }
           mon->hp_timer=0;
       }       
    }   
}
void Monster_draw(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    al_draw_bitmap(Obj->img,Obj->x,Obj->y,0);
}
void Monster_destory(Elements *const ele){
    Monster *Obj = ((Monster *)(ele->pDerivedObj));
    free(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}