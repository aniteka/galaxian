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

    bool init() override;
    void onExit() override;

    EnemyFactory* getEnemyFactory() { return enemyFactory; }
    PlayerShip* getPlayerShip() { return playerShip; }

    void updateGameplayMenu();

    void setupMainMenu();
    void setupGameplay();

    void releaseMainMenu();
    void releaseGameplay();

    void gameplayEnd(float interval);
    bool isGameplayEnd() const { return gameplayEnds; }

    void restartEnemyFactory();

    cocos2d::Layer* getMainMenuLayer() const { return mainMenu; };
    cocos2d::Layer* getGameplayLayer() const { return gameplayLayer; };

    CREATE_FUNC(MainScene);

protected:
    void gameplayEndCallback(float delay);

    void bgMoving();

protected:
    /*
     * MainMenu
     */
    cocos2d::Layer* mainMenu = nullptr;

    /*
     * Gameplay
     */
    cocos2d::Layer* gameplayLayer = nullptr;
    EnemyFactory* enemyFactory = nullptr;
    PlayerShip* playerShip = nullptr;
    cocos2d::Sprite* backgroundSprite = nullptr;

    cocos2d::Label* scoreLabel = nullptr;
    cocos2d::Label* hpLabel = nullptr;

    bool gameplayEnds = false;
    float maxScore = 0.f;
};

#endif // __HELLOWORLD_SCENE_H__
