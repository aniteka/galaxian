#include <iostream>
#include "EnemyShip.h"
#include "Utilities.h"
#include "MainScene.h"
#include "Actors/PlayerShip.h"
#include "EnemyProjectile.h"

USING_NS_CC;

EnemyShip* EnemyShip::createEnemyShip(EnemyType inEnemyType)
{
    auto pRet = new(std::nothrow)EnemyShip();
    if (pRet)
    {
        pRet->enemyType = inEnemyType;
    }

    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
    return pRet;
}

bool EnemyShip::init()
{
    const char* enemyShipFile;
    switch (enemyType)
    {
        case EnemyType::Blue:
            enemyShipFile = ENEMY_BLUE_SPRITE;
            break;
        case EnemyType::Purple:
            enemyShipFile = ENEMY_PURPLE_SPRITE;
            break;
        case EnemyType::Red:
            enemyShipFile = ENEMY_RED_SPRITE;
            break;
        case EnemyType::Flagship:
            enemyShipFile = ENEMY_FLAGSHIP_SPRITE;
            break;
        case EnemyType::None:
        default:
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyType));
            return false;
    }

    if (!Sprite::initWithFile(enemyShipFile))
    {
        return false;
    }

    this->setTag(ENEMY_SHIP_TAG);

    const auto physicsBody = PhysicsBody::createBox(Size(
            this->getVisibleSizeWidth(),
            this->getVisibleSizeHeight()));
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    this->setPhysicsBody(physicsBody);

    this->scheduleUpdate();
    return true;
}

void EnemyShip::update(float delta)
{
    Node::update(delta);

    if (isLaunched && this->getPosition().y > 100)
    {
        rotateToPlayerShipUpdate(delta);
        shootingUpdate(delta);
    }
}

float EnemyShip::getVisibleSizeWidth() const
{
    return this->getContentSize().width * this->getScaleX();
}

float EnemyShip::getVisibleSizeHeight() const
{
    return this->getContentSize().height * this->getScaleY();
}

void EnemyShip::setEnable(bool inIsEnable)
{
    _isEnable = inIsEnable;
    this->setVisible(isEnable());
    this->getPhysicsBody()->setEnabled(isEnable());

    if(isLaunched)
    {
        this->removeFromParent();
    }
}

void EnemyShip::launch()
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

    playerShip = playerShip ? playerShip : mainScene->getPlayerShip();
    if (!playerShip)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
        return;
    }

    const float mul = random<int>(0, 1) ? 1.f : -1.f;

    ccBezierConfig bezierConfig;
    bezierConfig.controlPoint_1 = Vec2(200 * mul, 100);
    bezierConfig.controlPoint_2 = (playerShip->getPosition() - this->getPosition()) + Vec2(100 * mul, 400);
    bezierConfig.endPosition = playerShip->getPosition() - this->getPosition() - Vec2(0, 100);
    auto bb = BezierBy::create(5, bezierConfig);

    this->runAction(bb);

    isLaunched = true;
}

void EnemyShip::rotateToPlayerShipUpdate(float delta)
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

    playerShip = playerShip ? playerShip : mainScene->getPlayerShip();
    if (!playerShip)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
        return;
    }

    rotateToPoint(playerShip->getPosition());
}

void EnemyShip::shootingUpdate(float delta)
{
    if(currentShootingTimeout > shootingTimeout)
    {
        currentShootingTimeout = 0.f;
        const auto director = Director::getInstance();
        if (!director)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
            return;
        }

        const auto mainScene = dynamic_cast<MainScene *>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
        if (!mainScene)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainScene));
            return;
        }

        playerShip = playerShip ? playerShip : mainScene->getPlayerShip();
        if (!playerShip)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
            return;
        }

        const auto enemyProjectile = EnemyProjectile::create();
        if(enemyProjectile)
        {
            mainScene->addChild(enemyProjectile);

            enemyProjectile->setPosition(this->getPosition());
            enemyProjectile->setRotation(this->getRotation());

            enemyProjectile->launch(playerShip->getPosition());
        }
    }
    currentShootingTimeout += delta;
}

void EnemyShip::rotateToPoint(cocos2d::Point point)
{
    float nodeCurrentAngle = this->getRotation();
    if (nodeCurrentAngle >= 180.f)
    {
        nodeCurrentAngle -= 360.f;
    } else if (nodeCurrentAngle < -180.f)
    {
        nodeCurrentAngle += 360.f;
    }

    auto vectorFromVec1ToVec2 = this->getPosition() - point;
    float angleNodeToRotateTo = CC_RADIANS_TO_DEGREES(-vectorFromVec1ToVec2.getAngle());

    float diffAngle = (angleNodeToRotateTo - nodeCurrentAngle);
    if(diffAngle >= 180.f)
    {
        diffAngle -= 360.f;
    }
    else if (diffAngle < -180.f)
    {
        diffAngle +=360.f;
    }

    float rotation = nodeCurrentAngle + diffAngle + 90.f;

    this->setRotation(rotation);
}

