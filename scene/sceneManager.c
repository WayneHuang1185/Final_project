#include "sceneManager.h"
#include "menu.h"
#include "gamescene.h"
#include "GameEnd.h"
Scene *scene = NULL;
void create_scene(SceneType type,int id)
{
    switch (type)
    {
    case Menu_L:
        scene = New_Menu(Menu_L);
        break;
    case GameScene_L:
        scene = New_GameScene(GameScene_L);
        break;
    case GameEnd_L:
        scene = New_GameEnd(GameEnd_L,id);
        break;
    default:
        break;
    }
}