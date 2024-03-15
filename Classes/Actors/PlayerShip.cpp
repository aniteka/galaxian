#include <iostream>
#include "PlayerShip.h"
#include "Projectile.h"
#include "Utilities.h"

USING_NS_CC;

cocos2d::Sprite *PlayerShip::createSprite()
{
    return PlayerShip::create();
}

bool PlayerShip::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    setupKeyboard();

    setupBody();

    setupProjectileView();

    this->scheduleUpdate();
    return true;
}

void PlayerShip::update(float delta)
{
    Sprite::update(delta);

    movingUpdate();
}


float PlayerShip::getVisibleBodySizeWidth() const
{
    return body->getContentSize().width * body->getScaleX();
}

float PlayerShip::getVisibleBodySizeHeight() const
{
    return body->getContentSize().height * body->getScaleY();
}

void PlayerShip::setupBody()
{
    body = Sprite::create(BODY_SPRITE);
    if (!body)
    {
        std::cout << GENERATE_ERROR_MESSAGE(body);
        return;
    }
    addChild(body);

    body->setScale(0.1);
}

void PlayerShip::setupProjectileView()
{
    projectileView = Projectile::createProjectile();
    if(projectileView)
    {
        this->addChild(projectileView);

        auto pos = Vec2(0,0);
        pos.y = getVisibleBodySizeHeight() / 2.f;

        projectileView->setPosition(pos);
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(projectileView);
    }
}

void PlayerShip::setupKeyboard()
{
    keyboardListener = EventListenerKeyboard::create();
    if (!keyboardListener)
    {
        std::cout << GENERATE_ERROR_MESSAGE(keyboardListener);
        return;
    }

    keyboardListener->onKeyPressed = CC_CALLBACK_2(PlayerShip::onKeyPressedCallback, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(PlayerShip::onKeyReleasedCallback, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void PlayerShip::movingUpdate()
{
    const auto director = Director::getInstance();
    if (!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }

    const auto bodyHalfSize = getVisibleBodySizeWidth() / 2.f;

    if (movingLeftState)
    {
        auto newXPos = getPosition().x - speed;
        newXPos = clampf(newXPos,
                         0 + bodyHalfSize,
                         director->getVisibleSize().width - bodyHalfSize);
        setPosition(Vec2(newXPos, getPosition().y));
    }

    if (movingRightState)
    {
        auto newXPos = getPosition().x + speed;
        newXPos = clampf(newXPos,
                         0 + bodyHalfSize,
                         director->getVisibleSize().width - bodyHalfSize);
        setPosition(Vec2(newXPos, getPosition().y));
    }
}

void PlayerShip::shoot()
{
    if(projectileReal)
        return;

    const auto director = Director::getInstance();
    if(!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }
    const auto scene = director->getRunningScene();
    if(!scene)
    {
        std::cout << GENERATE_ERROR_MESSAGE(scene);
        return;
    }

    projectileReal = Projectile::createProjectile();
    if(projectileReal)
    {
        scene->addChild(projectileReal);

        auto pos = this->getPosition();
        pos.y += getVisibleBodySizeHeight() / 2.f;

        projectileReal->onHit = CC_CALLBACK_1(PlayerShip::onProjectileHit, this);

        projectileReal->setPosition(pos);

        projectileReal->launch();
    }
    else
    {
        std::cout << GENERATE_ERROR_MESSAGE(projectileReal);
    }

    if(projectileView)
    {
        projectileView->setVisible(false);
    }
}

void PlayerShip::onProjectileHit(cocos2d::Sprite* hitSprite)
{
    projectileReal = nullptr;
    projectileView->setVisible(true);
}

void PlayerShip::onKeyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    this->stopAllActions();

    switch ((int) keyCode)
    {
        case 26: // Left key
            movingLeftState = true;
            break;
        case 27: // Right key
            movingRightState = true;
            break;
        case 28: // Up key
            shoot();
    }
}

void PlayerShip::onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    this->stopAllActions();

    switch ((int) keyCode)
    {
        case 26: // Left key
            movingLeftState = false;
            break;
        case 27: // Right key
            movingRightState = false;
            break;
    }
}
