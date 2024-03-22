#include <iostream>
#include "EnemyShip.h"
#include "Utilities.h"
#include "MainScene.h"
#include "Actors/PlayerShip.h"
#include "EnemyProjectile.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

int EnemyShip::flyingSoundId                        = AudioEngine::INVALID_AUDIO_ID;
EnemyShip* EnemyShip::currentFlyingShipUsedSound    = nullptr;

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
    std::string sprite;
    const auto idleAnim = Animation::create();
    idleAnim->setDelayPerUnit(0.5f);
    idleAnim->setLoops(-1);
    switch (enemyType)
    {
        case EnemyType::Blue:
            sprite = StringUtils::format(ENEMY_BLUE_SPRITE, 1);
            for(int i = 1; i <= ENEMY_BLUE_COUNT; ++i)
                idleAnim->addSpriteFrameWithFile( StringUtils::format(ENEMY_BLUE_SPRITE, i) );
            givenScore = 100.f;
            break;
        case EnemyType::Purple:
            sprite = StringUtils::format(ENEMY_PURPLE_SPRITE, 1);
            for(int i = 1; i <= ENEMY_PURPLE_COUNT; ++i)
                idleAnim->addSpriteFrameWithFile( StringUtils::format(ENEMY_PURPLE_SPRITE, i) );
            givenScore = 200.f;
            break;
        case EnemyType::Red:
            sprite = StringUtils::format(ENEMY_RED_SPRITE, 1);
            for(int i = 1; i <= ENEMY_RED_COUNT; ++i)
                idleAnim->addSpriteFrameWithFile( StringUtils::format(ENEMY_RED_SPRITE, i) );
            givenScore = 300.f;
            break;
        case EnemyType::Flagship:
            sprite = StringUtils::format(ENEMY_FLAGSHIP_SPRITE, 1);
            for(int i = 1; i <= ENEMY_FLAGSHIP_COUNT; ++i)
                idleAnim->addSpriteFrameWithFile( StringUtils::format(ENEMY_FLAGSHIP_SPRITE, i) );
            givenScore = 500.f;
            break;
        case EnemyType::None:
        default:
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyType));
            return false;
    }

    if (!Sprite::initWithFile(sprite))
    {
        return false;
    }

    this->runAction(Animate::create(idleAnim));

    this->setTag(ENEMY_SHIP_TAG);

    this->setScale(3.125f);

    const auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    this->setPhysicsBody(physicsBody);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(EnemyShip::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    shootingTimeout = random<float>(1.f, 2.f);

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

    collisionUpdate(delta);
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
    if(const auto physicsBody = this->getPhysicsBody())
    {
        physicsBody->setEnabled(isEnable());
    }

    if(isLaunched && !isEnable())
    {
        if(currentFlyingShipUsedSound == this)
        {
            AudioEngine::stop(flyingSoundId);
            currentFlyingShipUsedSound = nullptr;
        }

        this->removeFromParent();
    }
}

void EnemyShip::launch(float dir /* = 0*/)
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

    const auto playerShip = mainScene->getPlayerShip();
    if (!playerShip)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
        return;
    }

    launch(playerShip->getPosition(), dir);
}

void EnemyShip::launch(cocos2d::Vec2 location, float dir /* = 0*/)
{
    float mul;
    if(dir == 0.f)
    {
        mul = random<int>(0, 1) ? 1.f : -1.f;
    }
    else
    {
        mul = dir;
    }

    ccBezierConfig bezierConfig;
    bezierConfig.controlPoint_1 = Vec2(200 * mul, 100);
    bezierConfig.controlPoint_2 = (location - this->getPosition()) + Vec2(100 * mul, 400);
    bezierConfig.endPosition = location - this->getPosition() - Vec2(0, 100);
    auto bb = BezierBy::create(5, bezierConfig);

    this->runAction(bb);

    isLaunched = true;

    AudioEngine::stop(flyingSoundId);
    flyingSoundId = AudioEngine::play2d(FLYING_SOUND);
    currentFlyingShipUsedSound = this;
}

void EnemyShip::receiveDamage()
{
    this->setEnable(false);

    AudioEngine::play2d(HIT_ENEMY_SOUND);
}


bool EnemyShip::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if(nodeA && nodeB
       && (nodeA->getTag() == ENEMY_SHIP_TAG || nodeB->getTag() == ENEMY_SHIP_TAG)
       && (nodeA->getTag() == PLAYER_SHIP_TAG || nodeB->getTag() == PLAYER_SHIP_TAG))
    {
        const auto self = dynamic_cast<EnemyShip*>(nodeA->getTag() == ENEMY_SHIP_TAG ? nodeA : nodeB);
        const auto player = dynamic_cast<PlayerShip*>(nodeA->getTag() == PLAYER_SHIP_TAG ? nodeA : nodeB);
        if(!self || !player)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(!self || !player));
            return false;
        }

        self->removeFromParent();
        AudioEngine::play2d(HIT_ENEMY_SOUND);

        player->receiveDamage();
    }

    return false;
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

    const auto playerShip = mainScene->getPlayerShip();
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
        shootingTimeout = random<float>(1.f, 2.f);
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

        const auto playerShip = mainScene->getPlayerShip();
        if (!playerShip)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
            return;
        }

        const auto enemyProjectile = EnemyProjectile::create();
        if(enemyProjectile)
        {
            mainScene->getGameplayLayer()->addChild(enemyProjectile);

            enemyProjectile->setPosition(this->getPosition());
            enemyProjectile->setRotation(this->getRotation());

            enemyProjectile->launch(playerShip->getPosition());
        }
    }
    currentShootingTimeout += delta;
}

void EnemyShip::collisionUpdate(float delta)
{
    const auto yPos = getParent()->convertToWorldSpace(this->getPosition()).y;
    if(yPos <= -50.f)
    {
        this->setEnable(false);
    }
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
