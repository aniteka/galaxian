#include <iostream>
#include "MainScene.h"
#include "Actors/PlayerShip.h"
#include "Actors/EnemyFactory.h"
#include "Utilities.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    const auto director = Director::getInstance();
    if(!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return false;
    }

    const auto backgroundSprite = Sprite::create("Background.png");
    if (backgroundSprite)
    {
        this->addChild(backgroundSprite, 0);
        backgroundSprite->setAnchorPoint(Vec2(0, 0));
        backgroundSprite->setScale(director->getVisibleSize().height
                                   / backgroundSprite->getContentSize().height);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(backgroundSprite);
    }

    playerShip = PlayerShip::createPlayerShip();
    if (playerShip)
    {
        this->addChild(playerShip, 1);

        playerShip->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 0.08f);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(playerShip);
    }

    enemyFactory = EnemyFactory::createEnemyFactory();
    if(enemyFactory)
    {
        this->addChild(enemyFactory);

        enemyFactory->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 0.85f);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(enemyFactory);
    }

    return true;
}
