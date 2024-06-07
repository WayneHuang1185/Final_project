#ifndef Monster_Property_H_INCLUDED
#define Monster_Property_H_INCLUDED
#include "element.h"
#include "../scene/gamescene.h" // for element label
#include <allegro5/allegro_image.h>
#include"monster.h"
/*
   [Monster_Property object]
*/
/*
typedef struct _MonsterProperty Monster_Property;
typedef void (*fptrsuperPower)(Monster_Property *const);
typedef void (*fptrUpdate)(Monster_Property *const, Monster *const);
struct _MonsterProperty
{   int Dizzness;
    int uniqueSkill_timer;
    int exploded_timer;
    int id;
    bool explode;
    bool blood;
    bool trigger;
    fptrsuperPower superPower;
    fptrUpdate Update;
};
Monster_Property *New_Monster_Property(int id);
void Monster_Property_Update(Monster_Property *const,Monster *const);
void Monster_Property_SuperPower(Monster_Property *const);
*/
#endif
