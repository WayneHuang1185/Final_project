#include "player.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../algif5/src/algif.h"
#include "bullet.h"
#include "monster.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include <stdio.h>
#include<string.h>
/*
   [Player function]
*/
Elements *New_Player(int label)
{  
    Player *pDerivedObj = (Player *)malloc(sizeof(Player));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    // load Player images
    pDerivedObj->img=al_load_bitmap("assets/image/player.png");
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/atk_sound.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());

    // initial the geometric information of Player
    memset(pDerivedObj->skill_level, 0, sizeof(int));
    strcpy(pDerivedObj->name,"Wayne");
    for(int i=0;i<10;i++){
        pDerivedObj->skill_level[i]=0;
    }
    pDerivedObj->level=1;
    pDerivedObj->move_speed=2;
    pDerivedObj->bullet_speed=1;
    pDerivedObj->bullet_damage=30;
    pDerivedObj->bullet_reload=300;
    pDerivedObj->bullet_mp_consumption=15;
    pDerivedObj->hp_max=300;
    pDerivedObj->hp=300;
    pDerivedObj->hp_recovery=5;
    pDerivedObj->mp_max=150;
    pDerivedObj->mp=150;
    pDerivedObj->mp_recovery=5;
    pDerivedObj->sp_recover_time=100;
    pDerivedObj->recover_time=100;
    pDerivedObj->exp=0;
    pDerivedObj->sp=0;
    //
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->timer_for_mphp=0;
    pDerivedObj->timer_for_bullet=0;
    pDerivedObj->timer_for_sp=0;
    pDerivedObj->total_timer=10000;
    pDerivedObj->x = 300;
    pDerivedObj->y = 300;
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 15, 0);
    pDerivedObj->angle=atan2(mouse.y- pDerivedObj->y,mouse.x-pDerivedObj->x);
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x,
                                        pDerivedObj->y,
                                        pDerivedObj->x + pDerivedObj->width,
                                        pDerivedObj->y + pDerivedObj->height);
    pDerivedObj->dir = false; // true: face to right, false: face to left
    pDerivedObj->show_information=false;
    pDerivedObj->show_information=false;
    pDerivedObj->update_change=false;
    // initial the animation component
    pObj->inter_obj[pObj->inter_len++] = Monster_L;
    pDerivedObj->wlk_state = p_STOP;
    pDerivedObj->atk_state = p_CEASE_FIRE;
    pDerivedObj->new_shot = 0;
    pObj->pDerivedObj = pDerivedObj;
    
    // setting derived object function
    pObj->Draw = Player_draw;
    pObj->Update = Player_update;
    pObj->Interact = Player_interact;
    pObj->Destroy = Player_destory;
    return pObj;
}
/*
    moveSpeed,
    bulletSpeed,
    bulletDamage,
    bulletReload,
    hpRecovery,
    mpRecovery,
    hpMax,
    mpMax,
*/
void _Player_limit_timer(Elements *const ele){
    Player* chara=((Player*)(ele->pDerivedObj));
    chara->timer_for_bullet%=chara->total_timer;
    chara->timer_for_mphp%=chara->total_timer;
    chara->timer_for_sp%=chara->total_timer;
}

void _Player_mphp_recover(Elements *const ele)
{
     Player* chara=((Player*)(ele->pDerivedObj));
     if(chara->timer_for_mphp>=chara->recover_time){
        if(chara->mp<chara->mp_max){
           chara->mp+=chara->mp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE){
              chara->mp+=10;
           }
           chara->mp=(chara->mp>chara->mp_max)?chara->mp_max:chara->mp;
        }
        if(chara->hp<chara->hp_max){
           chara->hp+=chara->hp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE){
              chara->hp+=5;
           }
           chara->hp=(chara->hp > chara->hp_max)?chara->hp_max:chara->hp;
        }
        chara->timer_for_mphp%=chara->recover_time;
     }
}
void _Player_sp_update(Elements *const ele){
    Player* chara=((Player*)(ele->pDerivedObj));
    if(chara->sp>0){
        chara->update_change=true;
        if(key_state[ALLEGRO_KEY_1] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[moveSpeed]<6){
                chara->skill_level[moveSpeed]+=1;
                chara->sp-=1;
           }
           chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_2] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[bulletSpeed]<9){
                chara->skill_level[bulletSpeed]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_3] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[bulletDamage]<9){
                chara->skill_level[bulletDamage]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_4] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[bulletReload]<9){
                chara->skill_level[bulletReload]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_5] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[hpRecovery]<9){
                chara->skill_level[hpRecovery]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_6] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[mpRecovery]<9){
                chara->skill_level[mpRecovery]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_7] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[hpMax]<9){
                chara->skill_level[hpMax]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else if(key_state[ALLEGRO_KEY_8] && chara->timer_for_sp >= chara->sp_recover_time){
           if(chara->skill_level[mpMax]<9){
                chara->skill_level[mpMax]+=1;
                chara->sp-=1;
            }   
            chara->timer_for_sp%=chara->sp_recover_time;
        }
        else{
            chara->update_change=false;
        }
    }   

}
void Player_update(Elements *const ele)
{    
    // use the idea of finite state machine to deal with different state
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->timer_for_bullet+=5;
    chara->timer_for_mphp+=5;
    chara->timer_for_sp+=5;
    chara->exp+=50;
    int dx=chara->x,dy=chara->y;
    if (chara->wlk_state == p_STOP)
    {
        
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            chara->wlk_state = p_MOVE;
        }
        if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            chara->wlk_state = p_MOVE;
        }
        if (key_state[ALLEGRO_KEY_W])
        {
            chara->wlk_state=p_MOVE;
        }
        if(key_state[ALLEGRO_KEY_S])
        {   
            chara->wlk_state=p_MOVE;
        }
        if (key_state[ALLEGRO_KEY_SPACE] || mouse_state[1])
        {
            chara->atk_state = p_FIRE;
        }
    }
    else if (chara->wlk_state == p_MOVE || chara->wlk_state == p_FIRE )
    {   
        if (chara->atk_state == p_FIRE)
        {   
            if (chara->timer_for_bullet >= chara->bullet_reload && chara->mp>=chara->bullet_damage){
                
                double mx=mouse.x,my=mouse.y;
                double angle = atan2(my-chara->y,mx-chara->x);
                Elements *bullet;
                bullet=New_Bullet(Bullet_L,chara->x,chara->y,angle,chara->bullet_speed,chara->bullet_damage);
                _Player_update_position(ele, -chara->bullet_speed * cos(angle) , -chara->bullet_speed  * sin(angle));
                _Register_elements(scene,bullet);
                chara->atk_state = p_CEASE_FIRE;
                chara->timer_for_bullet%=chara->bullet_reload;
                chara->mp-=chara->bullet_mp_consumption;
            }
            else{
                chara->atk_state = p_CEASE_FIRE; 
            }
        }
        
        if (key_state[ALLEGRO_KEY_A])
        {
            chara->dir = false;
            _Player_update_position(ele, -chara->move_speed, 0);
            chara->wlk_state = p_MOVE;
        }
        if (key_state[ALLEGRO_KEY_D])
        {
            chara->dir = true;
            _Player_update_position(ele, chara->move_speed, 0);
            chara->wlk_state = p_MOVE;
        }
        if (key_state[ALLEGRO_KEY_W]){
            chara->wlk_state=p_MOVE;
            _Player_update_position(ele, 0,-chara->move_speed);
        }
        if(key_state[ALLEGRO_KEY_S]){
            chara->wlk_state=p_MOVE;
            _Player_update_position(ele, 0,chara->move_speed);
        }
        if (key_state[ALLEGRO_KEY_SPACE] || mouse_state[1])
        {
            chara->atk_state = p_FIRE;
        }
    }
    if((chara->x-dx)!=0 && (chara->y-dy)!=0){
       _Player_update_position(ele,-(chara->x-dx)/2,-(chara->y-dy)/2);
    }
    if(chara->wlk_state != p_MOVE && chara->wlk_state != p_FIRE)
        chara->wlk_state = p_STOP;
    
    if(key_state[ALLEGRO_KEY_TAB])
        chara->show_information=true;
    else 
        chara->show_information=false;
    _Player_sp_update(ele);
    _Player_mphp_recover(ele);
    _Player_limit_timer(ele);
    chara->angle=atan2(mouse.y- chara->y,mouse.x-chara->x);
}
void Player_draw(Elements *const ele)
    {
        char skill[10][20] = {
            "moveSpeed",
            "bulletSpeed",
            "bulletDamage",
            "bulletReload",
            "hpRecovery",
            "mpRecovery",
            "hpMax",
            "mpMax"
        };
        // with the state, draw corresponding image
        Player *Obj = ((Player *)(ele->pDerivedObj));
        int w = al_get_text_width(Obj->font, Obj->name)/2+5;
        al_draw_rotated_bitmap(Obj->img,Obj->width/2,Obj->height/2,Obj->x,Obj->y,Obj->angle+2.355,0);
        al_draw_filled_rectangle(Obj->x-w, Obj->y-Obj->height/2, Obj->x+w, Obj->y-Obj->height/2+20, al_map_rgba(0,0,0,100));
        al_draw_text(Obj->font, al_map_rgb(255,255,255), Obj->x, Obj->y - Obj->height/2, ALLEGRO_ALIGN_CENTRE, Obj->name);
        char tmp[50];
        if(Obj->show_information || true){
            for(int i = 0; i < 8; i++){
                sprintf(tmp, "%s: %d", skill[i], Obj->skill_level[i+1]+1); 
                al_draw_text(Obj->font, al_map_rgb(255,255,255), 0, 20*i, ALLEGRO_ALIGN_LEFT, tmp);
            }
            sprintf(tmp,"mp:%d",Obj->mp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,20*8,ALLEGRO_ALIGN_LEFT,tmp);
            sprintf(tmp,"hp:%d",Obj->hp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,20*9,ALLEGRO_ALIGN_LEFT,tmp);
        }
        
    }
void Player_destory(Elements *const ele)
{
    Player *Obj = ((Player *)(ele->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}

void _Player_update_position(Elements *const ele, int dx, int dy)
{
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->x += dx;
    chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Player_interact(Elements *const self, Elements *const target) {
    Player *pl=((Player*)(self->pDerivedObj));
    if(target->label == Monster_L){
        Monster *mon=((Monster*)(target->pDerivedObj));
        if(mon->hitbox->overlap(mon->hitbox,pl->hitbox) && mon->atk_timer>=mon->atk_frequency){
            if(pl->hp>mon->damage){
                pl->hp-=mon->damage;
            }
            else{
                self->dele=true;
            }
            mon->atk_timer%=mon->atk_frequency;
            pl->timer_for_mphp=0;
        }
    }
}


