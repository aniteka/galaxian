#include "EnemyFactory.h"
#include "EnemyShip.h"
#include "Utilities.h"
#include "MainScene.h"
#include <iostream>

USING_NS_CC;

bool EnemyFactory::init()
{
    if (!Node::init())
    {
        return false;
    }
    const auto director = Director::getInstance();
    if (!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return false;
    }

    float offsetY = 30.f;
    for (int i = 0; i < 3; ++i)
    {
        this->spawnEnemyRow(10, -offsetY * (float)i, EnemyType::Blue);
    }
    this->spawnEnemyRow(8, offsetY, EnemyType::Purple);
    this->spawnEnemyRow(6, offsetY * 2.f, EnemyType::Red);

    const auto offset = director->getVisibleSize().width * offsetMul;
    const auto flagship1 = this->spawnEnemyShip(EnemyType::Flagship);
    if(flagship1)
    {
        flagship1->setPosition(Vec2(offset / 2.f + offset,
                                    offsetY * 3.f));
    }

    const auto flagship2 = this->spawnEnemyShip(EnemyType::Flagship);
    if(flagship2)
    {
        flagship2->setPosition(Vec2(-(offset / 2.f + offset),
                                    offsetY * 3.f));
    }

    this->schedule(CC_SCHEDULE_SELECTOR(EnemyFactory::respawnShipInIntervalCallback), respawnInterval, countOfRespawns, respawnInterval);

    this->scheduleUpdate();
    return true;
}

void EnemyFactory::update(float delta)
{
    Node::update(delta);

    moveUpdate(delta);
    launchRandomShipUpdate(delta);
}

void EnemyFactory::onExit()
{
    Node::onExit();

    for (const auto item: respawningCurrentlyMovingShips)
    {
        item->removeFromParent();
    }
}

void EnemyFactory::moveUpdate(float interval)
{
    const auto director = Director::getInstance();
    if (!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    if (getPositionX() >= (director->getVisibleSize().width / 2.f) + 36.f
        || getPositionX() <= (director->getVisibleSize().width / 2.f) - 36.f)
    {
        movingRight = !movingRight;
        if (movingRight)
            movingDirMul = 1.f;
        else if (!movingRight)
            movingDirMul = -1.f;

        this->setPositionY(this->getPositionY() - movingDownVal);
    }
    this->setPositionX(this->getPositionX() + movingSpeed * movingDirMul);
}

void EnemyFactory::launchRandomShipUpdate(float interval)
{
    currentInterval += interval;
    if(currentInterval > nextInterval)
    {
        nextInterval = random<float>(2.f, 5.f);
        currentInterval = 0.f;

        decltype(enemyShips) enabledEnemyShips;
        std::copy_if(enemyShips.begin(), enemyShips.end(), std::back_inserter(enabledEnemyShips),
                     [](const EnemyShip* enemyShip)
                     {
                         return enemyShip->isEnable();
                     });

        if(enabledEnemyShips.empty())
        {
            const auto director = Director::getInstance();
            if (!director)
            {
                CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
                return;
            }

            const auto mainScene = dynamic_cast<MainScene*>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
            if (!mainScene)
            {
                CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainScene));
                return;
            }

            if(!mainScene->isGameplayEnd())
            {
                mainScene->restartEnemyFactory();
            }
            return;
        }

        auto& ship = enabledEnemyShips[random<int>(0, enabledEnemyShips.size() - 1)];
        auto toLaunch = this->spawnGameplayCopyOfEnemy(ship);
        toLaunch->launch();
        ship->setEnable(false);
    }
}

EnemyShip* EnemyFactory::spawnEnemyShip(EnemyType enemyType)
{
    const auto enemyShip = EnemyShip::createEnemyShip(enemyType);
    if (!enemyShip)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyShip));
        return nullptr;
    }
    this->addChild(enemyShip);
    enemyShips.push_back(enemyShip);

    return enemyShip;
}

void EnemyFactory::spawnEnemyRow(int count, const float y, EnemyType enemyType)
{
    const auto director = Director::getInstance();
    if (!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    for (int j = 0; j < count; ++j)
    {
        const auto ship = this->spawnEnemyShip(enemyType);
        if(!ship)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(ship));
            continue;
        }

        const auto wWidth = director->getVisibleSize().width;
        const auto offset = wWidth * offsetMul;
        const auto shipWidth = ship->getVisibleSizeWidth();

        const auto x =
                ((offset * (float)j)
                 - (wWidth / 2.f))
                + ((wWidth - ( shipWidth*(float)count + (offset - shipWidth)*((float)(count - 1)) ) ) / 2.f + shipWidth / 2.f);

        ship->setPosition(x, y);
    }
}

EnemyShip* EnemyFactory::spawnGameplayCopyOfEnemy(EnemyShip *toCopy) const
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return nullptr;
    }

    const auto mainScene = dynamic_cast<MainScene*>(director->getRunningScene()->getChildByName(MAIN_SCENE_NAME));
    if(!mainScene)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainScene));
        return nullptr;
    }

    const auto enemyShip = EnemyShip::createEnemyShip(toCopy->getEnemyType());
    if (!enemyShip)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyShip));
        return nullptr;
    }
    mainScene->getGameplayLayer()->addChild(enemyShip);

    enemyShip->setPosition(this->convertToWorldSpace(toCopy->getPosition()));

    return enemyShip;
}

void EnemyFactory::respawnShipInIntervalCallback(float interval)
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    decltype(enemyShips) enabledDisabledShips;
    std::copy_if(enemyShips.begin(), enemyShips.end(), std::back_inserter(enabledDisabledShips),
                 [this](const EnemyShip* enemyShip)
                 {
                     return !enemyShip->isEnable() && respawningCurrentlyFixedShips.find(enemyShip) == respawningCurrentlyFixedShips.end();
                 });

    if(enabledDisabledShips.empty())
        return;

    const auto toRespawn = enabledDisabledShips[random<unsigned int>(0, enabledDisabledShips.size() - 1)];
    respawningCurrentlyFixedShips.insert(toRespawn);

    const auto ship = spawnGameplayCopyOfEnemy(toRespawn);
    respawningCurrentlyMovingShips.insert(ship);

    const auto destination = ship->getPosition();
    ship->setPosition(director->getVisibleSize().width / 2.f,director->getVisibleSize().height * 1.1f );

    ccBezierConfig bezierConfig;
    bezierConfig.controlPoint_1 = Vec2(0,  director->getVisibleSize().height * 0.9f - ship->getPosition().y);
    bezierConfig.controlPoint_2 = (destination - ship->getPosition()).x < 0
            ? Vec2(-director->getVisibleSize().width * 0.1f, -director->getVisibleSize().height * 0.1f)
            : Vec2(director->getVisibleSize().width * 0.1f, -director->getVisibleSize().height * 0.1f);
    bezierConfig.endPosition = destination - ship->getPosition();
    auto bb = BezierBy::create(5, bezierConfig);

    const auto cf = CallFunc::create([ship, toRespawn, this](){
        respawningCurrentlyMovingShips.erase(ship);
        respawningCurrentlyFixedShips.erase(toRespawn);
        ship->removeFromParent();
        if(ship->isEnable())
        {
            toRespawn->setEnable(true);
        }
    });

    ship->runAction( Sequence::create(bb, cf, nullptr) );
}

