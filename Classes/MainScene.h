#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class EnemyFactory;

class MainScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    EnemyFactory* getEnemyFactory() const { return enemyFactory; }

    CREATE_FUNC(MainScene);

protected:
    EnemyFactory* enemyFactory;
};

#endif // __HELLOWORLD_SCENE_H__
