
#include "../shapes/Circle.h"
#include"../global.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro_primitives.h>
#include<math.h>
#include"button.h"
const int Button_scaled=40;
Elements *New_Button(int label,double x, double y){
    Button *pDerivedObj=(Button*)malloc(sizeof(Button));
    Elements *pObj = New_Elements(label);
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 12, 0);
    pDerivedObj->press=false;

    char tmp[50];
    sprintf(tmp, "assets/image/button%d_nohurt.png",0);
    pDerivedObj->img[0] = al_load_bitmap(tmp);
    sprintf(tmp, "assets/image/button%d_nohurt.png",1);
    pDerivedObj->img[1] = al_load_bitmap(tmp);
    pDerivedObj->width=al_get_bitmap_width(pDerivedObj->img[0]);
    pDerivedObj->height=al_get_bitmap_height(pDerivedObj->img[0]);
    pDerivedObj->cx=x,pDerivedObj->cy=y;
    pObj->Update = Button_update;
    pObj->Interact = Button_interact;
    pObj->Draw = Button_draw;
    pObj->Destroy = Button_destory;
    return pObj;
}
void Button_update(Elements *const ele){
    Button*btn=((Button*)(ele->pDerivedObj));
    //determine the button state
    double mx=mouse.x,my=mouse.y;
    if(mx<=btn->cx+btn->width/2 && mx>=btn->cx-btn->width/2 
    && my>=btn->cy-btn->height/2 && my<=btn->cy+btn->height/2){
        if(mouse_state[1]){
            btn->press=true;
        }
        else{
            btn->press=false;
        }
    }
}
void Button_interact(Elements *const self, Elements *const ele){
     
}
void Button_draw(Elements *const ele){
    Button*btn=((Button*)(ele->pDerivedObj));
    al_draw_bitmap(btn->img[btn->press],btn->cx-btn->width/2,btn->cy-btn->height/2,0);
}
void Button_destory(Elements *const ele){
    Button *Obj = ((Button *)(ele->pDerivedObj));
    for(int i=0;i<2;i++)
        al_destroy_bitmap(Obj->img[i]);
    free(Obj);
    free(ele);
}