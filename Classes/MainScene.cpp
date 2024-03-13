#include <iostream>
#include "MainScene.h"
#include "Actors/PlayerShip.h"
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

    const auto backgroundSprite = Sprite::create("Background.png");
    if (backgroundSprite)
    {
        this->addChild(backgroundSprite, 0);
        backgroundSprite->setAnchorPoint(Vec2(0, 0));
        backgroundSprite->setScale(Director::getInstance()->getVisibleSize().height
                                   / backgroundSprite->getContentSize().height);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(backgroundSprite);
    }

    const auto playerShip = PlayerShip::createSprite();
    if (playerShip)
    {
        this->addChild(playerShip, 1);

        playerShip->setPosition(
                Director::getInstance()->getVisibleSize().width / 2.f,
                Director::getInstance()->getVisibleSize().height * 0.1f);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(playerShip);
    }

    return true;
}
