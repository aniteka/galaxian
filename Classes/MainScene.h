#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class PlayerShip;
class EnemyFactory;

#define MAIN_SCENE_NAME "MainScene"

class MainScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    EnemyFactory* getEnemyFactory() { return enemyFactory; }
    PlayerShip* getPlayerShip() { return playerShip; }

    CREATE_FUNC(MainScene);

protected:
    void setupMainMenu();
    void setupGameplay();

    void releaseMainMenu();
    void releaseGameplay();

protected:
    /*
     * MainMenu
     */
    cocos2d::Layer* mainMenu;

    /*
     * Gameplay
     */
    cocos2d::Layer* gameplayLayer;
    EnemyFactory* enemyFactory;
    PlayerShip* playerShip;
};

#endif // __HELLOWORLD_SCENE_H__
