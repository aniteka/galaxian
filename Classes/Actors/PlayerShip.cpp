#include "PlayerShip.h"
#include "Utilities.h"

USING_NS_CC;

cocos2d::Sprite* PlayerShip::createSprite()
{
    return PlayerShip::create();
}

bool PlayerShip::init()
{
    if(!Sprite::init())
    {
        return false;
    }

    if((body = Sprite::create("DEV/DevA.png")))
    {
        body->setScale(0.1);
        this->addChild(body);
    }
    else
    {
        perror(GENERATE_ERROR_MESSAGE(body));
    }

    return true;
}
