#include <iostream>
#include "PlayerShip.h"
#include "Projectile.h"
#include "Utilities.h"
#include "EnemyShip.h"
#include "MainScene.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

bool PlayerShip::init()
{
    if (!Sprite::init())
    {
        return false;
    }

    this->setTag(PLAYER_SHIP_TAG);

    setupKeyboard();

    setupBody();

    setupPhysicsBody();

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

void PlayerShip::receiveDamage()
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }
    const auto mainScene = dynamic_cast<MainScene*>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
    if(!mainScene)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainScene));
        return;
    }

    if(!mainScene->isGameplayEnd())
    {
        hp -= 1;
        if(hp < 0) hp = 0;

        mainScene->updateGameplayMenu();

        if (hp <= 0)
        {
            mainScene->gameplayEnd(1.f);
        }
    }
}

void PlayerShip::setupBody()
{
    body = Sprite::create(BODY_SPRITE);
    if (!body)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(body));
        return;
    }
    addChild(body);

    body->setScale(3.125f);
}

void PlayerShip::setupPhysicsBody()
{
    const auto physicsBody = PhysicsBody::createBox(Size(
            body->getContentSize().width * body->getScaleX(),
            body->getContentSize().height * body->getScaleY()));
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    this->setPhysicsBody(physicsBody);
}

void PlayerShip::setupProjectileView()
{
    projectileView = Projectile::create();
    if(projectileView)
    {
        this->addChild(projectileView);

        auto pos = Vec2(0,0);
        pos.y = getVisibleBodySizeHeight() / 2.f;

        projectileView->setPosition(pos);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(projectileView));
    }
}

void PlayerShip::setupKeyboard()
{
    keyboardListener = EventListenerKeyboard::create();
    if (!keyboardListener)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(keyboardListener));
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
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
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
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }
    const auto scene = dynamic_cast<MainScene*>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
    if(!scene)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(scene));
        return;
    }

    projectileReal = Projectile::create();
    if(projectileReal)
    {
        scene->getGameplayLayer()->addChild(projectileReal);

        auto pos = this->getPosition();
        pos.y += getVisibleBodySizeHeight() / 2.f;

        projectileReal->onHit = CC_CALLBACK_1(PlayerShip::onProjectileHit, this);

        projectileReal->setPosition(pos);

        projectileReal->launch();

        AudioEngine::play2d(SHOOT_SOUND);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(projectileReal));
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

    if(const auto ship = dynamic_cast<EnemyShip*>(hitSprite))
    {
        const auto director = Director::getInstance();
        if(!director)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
            return;
        }
        const auto mainScene = dynamic_cast<MainScene*>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
        if(!mainScene)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainScene));
            return;
        }

        if(!mainScene->isGameplayEnd())
        {
            score += ship->getScore();
            mainScene->updateGameplayMenu();
        }
    }
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

