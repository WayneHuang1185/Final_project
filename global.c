#include "global.h"
#include "shapes/Shape.h"
// variables for global usage
const double FPS = 60.0;
const int WIDTH = 1704;
const int HEIGHT = 910;
int player_skill_level[10]={0};
int window = 0;
char skill[10][20] = {
    "spGain",
    "moveSpeed",
    "bulletSpeed",
    "bulletDamage",
    "bulletReload",
    "hpRecovery",
    "mpRecovery",
    "hpMax",
    "mpMax",
    "expMax"
};
int monster_stage=0;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse;
bool debug_mode = true;
