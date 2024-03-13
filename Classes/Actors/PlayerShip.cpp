#include <iostream>
#include "PlayerShip.h"
#include "Utilities.h"

USING_NS_CC;

const char* const PlayerShip::BODY_SPRITE = "DEV/DevA.png";

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

    setupKeyboard();

    setupBody();

    this->scheduleUpdate();
    return true;
}

void PlayerShip::update(float delta)
{
    if(movingLeftState)
    {
        this->setPosition(Vec2(this->getPosition().x - 5, this->getPosition().y));
    }

    if(movingRightState)
    {
        this->setPosition(Vec2(this->getPosition().x + 5, this->getPosition().y));
    }
}

void PlayerShip::setupKeyboard()
{
    keyboardListener = EventListenerKeyboard::create();
    if(!keyboardListener)
    {
        std::cout << GENERATE_ERROR_MESSAGE(keyboardListener);
        return;
    }

    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerShip::onKeyPressedCallback, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerShip::onKeyReleasedCallback, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void PlayerShip::setupBody()
{
    body = Sprite::create(BODY_SPRITE);
    if(!body)
    {
        std::cout << GENERATE_ERROR_MESSAGE(body);
        return;
    }
    addChild(body);

    body->setScale(0.1);
}

void PlayerShip::onKeyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    this->stopAllActions();

    switch ((int)keyCode)
    {
    case 26:
        movingLeftState = true;
        break;
    case 27:
        movingRightState = true;
        break;
    }
}

void PlayerShip::onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    this->stopAllActions();


    switch ((int)keyCode)
    {
    case 26:
        movingLeftState = false;
        break;
    case 27:
        movingRightState = false;
        break;
    }
}
