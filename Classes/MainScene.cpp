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
    if ( !Scene::init() )
    {
        return false;
    }

    const auto backgroundSprite = Sprite::create("Background.png");
    if(backgroundSprite)
    {
        this->addChild(backgroundSprite, 0);
        backgroundSprite->setAnchorPoint(Vec2(0,0));
        backgroundSprite->setScale(0.534f);
    }
    else
    {
        perror(GENERATE_ERROR_MESSAGE(backgroundSprite));
    }

    const auto playerShip = PlayerShip::createSprite();
    if(playerShip)
    {
        this->addChild(playerShip, 1);

        playerShip->setPosition(Director::getInstance()->getVisibleSize() / 2);

        const auto a =  MoveTo::create(5, Director::getInstance()->getVisibleSize());
        const auto delay = DelayTime::create(5);
        const auto b =  MoveTo::create(5, Director::getInstance()->getVisibleSize() / 2);
        playerShip->runAction(Sequence::create(a, delay, b, nullptr));
    }
    else
    {
        perror(GENERATE_ERROR_MESSAGE(playerShip));
    }

    return true;
}
