#ifndef Player_H_INCLUDED
#define Player_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/src/algif.h"
#include <allegro5/allegro_font.h>
/*
   [Player object]
*/
typedef enum SkillType
{
    spGain=0,
    moveSpeed,
    bulletSpeed,
    bulletDamage,
    bulletReload,
    hpRecovery,
    mpRecovery,
    hpMax,
    mpMax,
    expMax
} SkillType;
typedef enum PlayerType
{
	p_STOP = 0,
	p_MOVE,
    p_CEASE_FIRE,
	p_FIRE
} PlayerType;

typedef struct _Player
{   //playerManager parameter
    char name[20];
    int skill_level[10];
    int move_speed; 
    int bullet_speed;
    int bullet_damage;
    int bullet_reload;
    int hp_recovery;
    int mp_recovery;
    int hp_max;
    int mp_max;
    int exp;
    int level;
    int sp;
    //
    int x, y;     
    int timer; 
    int width, height; // the width and height of image
    bool dir;    
    bool update_change;
    bool show_information;
    double angle;    
    int wlk_state;  
    int atk_state;      // the state of Player
    int new_shot;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    Shape *hitbox; // the hitbox of object
    ALLEGRO_BITMAP* img; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    ALLEGRO_FONT* font;
} Player;


Elements *New_Player(int label);
void _Player_update_position(Elements *const ele, int dx, int dy);
void _Player_sp_update(Elements *const ele);
void Player_update(Elements *const self);
void Player_interact(Elements *const self, Elements *const target);
void Player_draw(Elements *const self);
void Player_destory(Elements *const self);

#endif
