#include "player.h"
#include "../scene/sceneManager.h"
#include "../shapes/Rectangle.h"
#include "../shapes/Circle.h"
#include "../algif5/src/algif.h"
#include "bullet.h"
#include "monster.h"
#include <allegro5/allegro_primitives.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
/*
   [Player function]
*/
const int text_size=10;
const int text_sap=3;
Elements *New_Player(int label)
{  
    
    // setting derived object member
    Player *pDerivedObj = (Player *)malloc(sizeof(Player));
    Elements *pObj = New_Elements(label);
    
    // load Player images
    for(int i=0;i<10;i++)
        pDerivedObj->skill_level[i]=0;
    pDerivedObj->img[0]=al_load_bitmap("assets/image/player.png");
    pDerivedObj->img[1]=al_load_bitmap("assets/image/player2.png");
    pDerivedObj->r=al_get_bitmap_width(pDerivedObj->img[0])/2;
    // load effective sound
    ALLEGRO_SAMPLE *sample = al_load_sample("assets/sound/shot.wav");
    pDerivedObj->atk_Sound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(pDerivedObj->atk_Sound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(pDerivedObj->atk_Sound, al_get_default_mixer());
    // load font
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", text_size, 0);
    // initial the geometric information of Player
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img[0]);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img[0]);    
    pDerivedObj->x = HEIGHT/2, pDerivedObj->y = WIDTH/2;
    pDerivedObj->hitbox = New_Circle( pDerivedObj->x, pDerivedObj->y, pDerivedObj->width/2);

    memset(pDerivedObj->skill_level, 0, sizeof(pDerivedObj->skill_level));
    strcpy(pDerivedObj->name,"Wayne");

    // initialise properties
    pDerivedObj->level=1;
    pDerivedObj->bullet_mp_consumption=5;
    pDerivedObj->move_speed=1;
    pDerivedObj->bullet_reload=300;
    pDerivedObj->bullet_speed=2;
    pDerivedObj->hp=300;
    pDerivedObj->hp_max=300;
    pDerivedObj->mp=150;
    pDerivedObj->mp_max=150;
    pDerivedObj->sp_recover_time=100;
    pDerivedObj->recover_time=100;
    pDerivedObj->hp_recovery=5;
    pDerivedObj->mp_recovery=5;
    pDerivedObj->exp=0;
    pDerivedObj->sp=40;
    pDerivedObj->hurt=false;
    pDerivedObj->immortal=false;
    // initialise timers
    pDerivedObj->timer_for_immortal=180;
    pDerivedObj->timer_for_mphp=100;
    pDerivedObj->timer_for_bullet=300;
    pDerivedObj->anime_time=0;
    pDerivedObj->total_timer=10000;
    pDerivedObj->show_information=false;
    pDerivedObj->show_information_permanent=true;
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
}

void _Player_mphp_recover(Elements *const ele)
{
     Player* chara=((Player*)(ele->pDerivedObj));
     if(chara->timer_for_mphp>=chara->recover_time){
        if(chara->mp<chara->mp_max){
           chara->mp+=chara->mp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE) chara->mp+=10;
           chara->mp=(chara->mp>chara->mp_max)?chara->mp_max:chara->mp;
        }
        if(chara->hp<chara->hp_max){
           chara->hp+=chara->hp_recovery;
           if(chara->wlk_state == p_STOP && chara->atk_state == p_CEASE_FIRE) chara->hp+=5;
           chara->hp=(chara->hp > chara->hp_max)?chara->hp_max:chara->hp;
        }
        chara->timer_for_mphp%=chara->recover_time;
     }
}

void _Player_sp_update(Elements *const ele){
    Player* chara=((Player*)(ele->pDerivedObj));
    if(chara->sp > 0 && chara->anime_time == 0){
        chara->update_change=false;
        for(int i = 0; i < 8; i++)
            if(key_state[ALLEGRO_KEY_1 + i] && chara->skill_level[i+1] < 7)
                chara->skill_level[i+1]++, chara->sp--, chara->anime_time=60,chara->update_change=true;
    }
}

//50 200 115 50
//200 50  75
void Player_update(Elements *const ele)
{   
    // use the idea of finite state machine to deal with different state
    Player *chara = ((Player *)(ele->pDerivedObj));    
    //printf("%d\n",chara->timer_for_immortal);
    if(chara->x<=0){
        chara->hitbox->update_center_x(chara->hitbox,(0 - chara->x));
        chara->x=0;
    }
    else if(chara->x>=WIDTH){
        chara->hitbox->update_center_x(chara->hitbox,(WIDTH- chara->x));
        chara->x=WIDTH;
    }
    if(chara->y<=0){
        chara->hitbox->update_center_y(chara->hitbox,(0 - chara->y));
        chara->y=0;
    }
    else if(chara->y>=HEIGHT){
        chara->hitbox->update_center_y(chara->hitbox,(HEIGHT- chara->y));
        chara->y=HEIGHT;
    }
    if(chara->anime_time) chara->anime_time-=3;
    chara->timer_for_bullet+=5, chara->timer_for_mphp+=5;
    if(chara->immortal && chara->timer_for_immortal>0)
        chara->timer_for_immortal-=1;
    else if(chara->timer_for_immortal == 0){
        chara->timer_for_immortal=180;
        chara->immortal=false;
    }
    double origin_x=chara->x,origin_y=chara->y;

    if (chara->atk_state == p_FIRE && chara->timer_for_bullet >= chara->bullet_reload && chara->mp >= chara->bullet_damage){
        double mx=mouse.x,my=mouse.y;
        double r = 0.075;
        double angle = atan2(my-chara->y,mx-chara->x) + (double)(rand() % 200 - 100) / 50 * r;
        Elements *bullet;
        bullet=New_Bullet(Bullet_L,chara->x+chara->r*cos(angle),chara->y+chara->r*sin(angle),angle,chara->bullet_speed,chara->bullet_damage);
        _Player_update_position(ele, -chara->bullet_speed * cos(angle) , -chara->bullet_speed  * sin(angle));
        _Register_elements(scene,bullet);
        al_play_sample_instance(chara->atk_Sound);
        chara->timer_for_bullet%=chara->bullet_reload;
        chara->mp-=chara->bullet_mp_consumption;
    }
    
    // position update correct
    if (key_state[ALLEGRO_KEY_A]) _Player_update_position(ele, -chara->move_speed, 0);
    if (key_state[ALLEGRO_KEY_D]) _Player_update_position(ele, chara->move_speed, 0);
    if (key_state[ALLEGRO_KEY_W]) _Player_update_position(ele, 0, -chara->move_speed);
    if (key_state[ALLEGRO_KEY_S]) _Player_update_position(ele, 0, chara->move_speed);
    if (chara->x - origin_x != 0 && chara->y - origin_y != 0) _Player_update_position(ele, (origin_x-chara->x)/2, (origin_y-chara->y)/2);
    
    chara->angle=atan2(mouse.y-chara->y,mouse.x-chara->x);
    if (key_state[ALLEGRO_KEY_SPACE] || mouse_state[1]) chara->atk_state = p_FIRE;
    else chara->atk_state = p_CEASE_FIRE; 
    // determine whther to open skill board
    if(key_state[ALLEGRO_KEY_TILDE] || key_state[ALLEGRO_KEY_TAB] || key_state[ALLEGRO_KEY_LCTRL] || mouse_state[2])
        chara->show_information=true;
    else
        chara->show_information=false;

    if(key_state[ALLEGRO_KEY_O] && chara->anime_time==0){
        chara->show_information_permanent = chara->show_information_permanent ? false: true;
        chara->anime_time = 60;
    }

    _Player_sp_update(ele);
    _Player_mphp_recover(ele);
    _Player_limit_timer(ele);
    _Player_super_power(ele);
}

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
int hardColor[8][3] = 
    {{223, 122, 122},
    {223, 161, 122},
    {203, 223, 122},
    {122, 153, 223},
    {122, 205, 223},
    {122, 153, 223},
    {223, 122, 207},
    {223, 122, 177}};
int baseColor[3] = {172, 231, 232};
const int flash_region=200;
const int immortal_consumpution=300;
int distance(int x, int y){

    return x*x+y*y;
}
void _draw_flash_region(Elements *const ele){
    Player *chara = ((Player *)(ele->pDerivedObj));
    al_draw_filled_circle(chara->hitbox->center_x(chara->hitbox),chara->hitbox->center_y(chara->hitbox),min(chara->mp_max/4,flash_region), al_map_rgba(200,50,75,200));
    al_draw_filled_circle(chara->hitbox->center_x(chara->hitbox),chara->hitbox->center_y(chara->hitbox),min(chara->mp/4,flash_region), al_map_rgba(50,200,115,200));
}
void _Player_super_power(Elements *const ele){ 
    Player *chara = ((Player *)(ele->pDerivedObj));
    if(key_state[ALLEGRO_KEY_F]){
       int dx=mouse.x-chara->hitbox->center_x(chara->hitbox),dy=mouse.y-chara->hitbox->center_y(chara->hitbox);
       double dis=sqrt(distance(dx,dy));
       _draw_flash_region(ele);
       if((int)dis*4<=chara->mp && (int)dis<=flash_region && mouse_state[2]){
          chara->mp-=(int)dis*4;
          _Player_update_position(ele,dx,dy);
       }
    }
    if(key_state[ALLEGRO_KEY_G]){
        if(chara->mp>=immortal_consumpution && !chara->immortal){
            chara->mp-=immortal_consumpution;
            chara->immortal=true;
        }
    }
    
}
void Player_draw(Elements *const ele)
    {
        int x_init = 20, y_init = HEIGHT-170;
        // with the state, draw corresponding image
        _Player_super_power(ele);
        Player *Obj = ((Player *)(ele->pDerivedObj));
        int w = al_get_text_width(Obj->font, Obj->name)/ 2+5;
      
        if(Obj->immortal)
            al_draw_tinted_rotated_bitmap(Obj->img[0],al_map_rgba(143, 225, 79, 200),
        Obj->width/2,Obj->height/2,Obj->x,Obj->y, Obj->angle+2.355, 0);
        else{
            al_draw_rotated_bitmap(Obj->img[Obj->hurt],Obj->width/2,Obj->height/2,Obj->x,Obj->y,Obj->angle+2.355,0);
        }
        al_draw_text(Obj->font, al_map_rgb(255,255,255),Obj->x, Obj->y-Obj->height/2, ALLEGRO_ALIGN_CENTRE, Obj->name);
        char tmp[50];
        if(Obj->show_information || Obj->show_information_permanent){
            al_draw_filled_rectangle(x_init-5, y_init-5, x_init+18*7+16+5, y_init + 22*7+15+5, al_map_rgba(0,0,0,80));
            al_draw_filled_rectangle(x_init-5, y_init-5+300, x_init+18*7+16+5, y_init + 22*7+15+5+300, al_map_rgba(0,0,0,80));
            for(int i = 0; i < 8; i++){
                sprintf(tmp, "%s: %d", skill[i], Obj->skill_level[i+1]+1); 
                al_draw_text(Obj->font, al_map_rgb(255,255,255), 0, (text_size+text_sap)*i, ALLEGRO_ALIGN_LEFT, tmp);
                for(int j = 0; j < 8; j++){
                    al_draw_filled_rectangle(x_init+18*i, y_init+22*j, x_init+18*i+16, y_init + 22*j+15, 
                    i <= Obj -> skill_level[j+1] ? al_map_rgb(hardColor[j][0],hardColor[j][1],hardColor[j][2]) : 
                    al_map_rgb(baseColor[0],baseColor[1],baseColor[2]));
                }
            }
            sprintf(tmp,"mp:%d",Obj->mp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,(text_size+text_sap)*9,ALLEGRO_ALIGN_LEFT,tmp);
            sprintf(tmp,"hp:%d",Obj->hp);
            al_draw_text(Obj->font,al_map_rgb(255,255,255),0,(text_size+text_sap)*8,ALLEGRO_ALIGN_LEFT,tmp);
        }
        Obj->hitbox->draw_hitbox(Obj->hitbox);
    }
void Player_destory(Elements *const ele)
{
    Player *Obj = ((Player *)(ele->pDerivedObj));
    al_destroy_sample_instance(Obj->atk_Sound);
    for(int i=0;i<2;i++)
        al_destroy_bitmap(Obj->img[i]);
    free(Obj->hitbox);
    free(Obj);
    free(ele);
}

void _Player_update_position(Elements *const ele, double dx, double dy)
{
    Player *chara = ((Player *)(ele->pDerivedObj));
    chara->x +=dx, chara->y += dy;
    Shape *hitbox = chara->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}

void Player_interact(Elements *const self, Elements *const target) {
    Player *pl=((Player*)(self->pDerivedObj));

    if(target->label == Monster_L){
        Monster *mon=((Monster*)(target->pDerivedObj));
        if(!pl->immortal && mon->hitbox->overlap(mon->hitbox,pl->hitbox) && mon->atk_timer>=mon->atk_frequency){
            if(pl->hp>mon->damage) pl->hp-=mon->damage;
            else{self->dele=true;scene->scene_end=true;window=3;}
            mon->atk_timer%=mon->atk_frequency;
            pl->timer_for_mphp=0;
        }
    }

}


