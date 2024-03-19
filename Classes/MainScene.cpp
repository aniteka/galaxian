#include <iostream>
#include "MainScene.h"
#include "Actors/PlayerShip.h"
#include "Actors/EnemyFactory.h"
#include "Utilities.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    const auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = MainScene::create();
    scene->addChild(layer, 0, MAIN_SCENE_NAME);
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    setupMainMenu();
    //setupGameplay();

    return true;
}

void MainScene::setupMainMenu()
{
    mainMenu = Layer::create();
    this->addChild(mainMenu, 1);

    const auto buttonStart = ui::Button::create(START_BUTTON_SPRITE,START_BUTTON_SPRITE,START_BUTTON_SPRITE);
    buttonStart->setPosition(Vec2(300, 340));
    buttonStart->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            setupGameplay();
            releaseMainMenu();
        }
    });
    mainMenu->addChild(buttonStart);

    const auto buttonExit = ui::Button::create(EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE);
    buttonExit->setPosition(Vec2(300, 260));
    buttonExit->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
        if(type == ui::Widget::TouchEventType::ENDED)
        {
            const auto director = Director::getInstance();
            if(director)
            {
                director->end();
            }
        }
    });
    mainMenu->addChild(buttonExit);
}

void MainScene::setupGameplay()
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    gameplayLayer = Layer::create();
    this->addChild(gameplayLayer);

    const auto backgroundSprite = Sprite::create("Background.png");
    if (backgroundSprite)
    {
        gameplayLayer->addChild(backgroundSprite, 0);
        backgroundSprite->setAnchorPoint(Vec2(0, 0));
        backgroundSprite->setScale(director->getVisibleSize().height
                                   / backgroundSprite->getContentSize().height);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(backgroundSprite));
    }

    playerShip = PlayerShip::create();
    if (playerShip)
    {
        gameplayLayer->addChild(playerShip, 1);

        playerShip->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 0.08f);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
    }

    enemyFactory = EnemyFactory::create();
    if(enemyFactory)
    {
        gameplayLayer->addChild(enemyFactory);

        enemyFactory->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 0.85f);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyFactory));
    }
}

void MainScene::releaseMainMenu()
{
    mainMenu->removeFromParent();
    mainMenu = nullptr;
}

void MainScene::releaseGameplay()
{
    gameplayLayer->removeFromParent();
    gameplayLayer = nullptr;
    enemyFactory = nullptr;
    playerShip = nullptr;
}

