#include"monster_property.h"
/*
Monster_Property *New_Monster_Property(int id){
       Monster_Property *mp=((Monster_Property*)malloc(sizeof(Monster_Property)));
       mp->id=id;
       mp->explode=false;
       mp->blood=false;
       mp->trigger=false; 
       mp->exploded_timer=0;
       mp->uniqueSkill_timer=0;
       mp->Dizzness=0;
       mp->Update=Monster_Property_Update;
       mp->superPower=Monster_Property_SuperPower;
       switch(id){
            case 0:
                mp->trigger=true;
                mp->exploded_timer=180;
            case 1:
                mp->blood=true;
            case 2:
                break;
       }
       return mp;
}
void Monster_Property_Update(Monster_Property *const self, Monster * ele){
     int id=self->id;
     switch(id){
        case 0:
            if(self->exploded_timer >=0 && ele->touch)
                self->exploded_timer-=1;
            else{
                self->exploded_timer=180;
            }
        case 1:
            break;
        case 2:
            break;
        case 3:
            if(self->uniqueSkill_timer>=0)
                self->uniqueSkill_timer-=1;
            else if(!self->trigger){
                self->trigger=true;
                self->uniqueSkill_timer=300;
            }
    }
}
void UniqueSkill(Monster_Property *const self,Elements *ele){

}
void Monster_Property_SuperPower(Monster_Property *const self){
     int id=self->id;
     switch(id){
        case 0:
            if(self->exploded_timer <= 0){
                self->explode=true;
            }
        case 1:
            break;
        case 2:
            break;
        case 3:
            if(self->uniqueSkill_timer<=0){
                self->trigger=false;
            }
     }
}*/