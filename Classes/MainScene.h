#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class PlayerShip;
class EnemyFactory;

class MainScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    EnemyFactory* getEnemyFactory() { return enemyFactory; }
    PlayerShip* getPlayerShip() { return playerShip; }

    CREATE_FUNC(MainScene);

protected:
    EnemyFactory* enemyFactory;
    PlayerShip* playerShip;
};

#endif // __HELLOWORLD_SCENE_H__
