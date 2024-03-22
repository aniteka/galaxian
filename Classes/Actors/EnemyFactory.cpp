#include "EnemyFactory.h"
#include "EnemyShip.h"
#include "Utilities.h"
#include "MainScene.h"
#include "PlayerShip.h"
#include <iostream>

USING_NS_CC;

template <typename T> T CLAMP(const T& value, const T& low, const T& high)
{
    return value < low ? low : (value > high ? high : value);
}

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
    this->spawnEnemyRow(6, offsetY * 3.f, EnemyType::Flagship);

    enemyShipsByTypes[EnemyType::Flagship][0]->setEnable(false);
    enemyShipsByTypes[EnemyType::Flagship][2]->setEnable(false);
    enemyShipsByTypes[EnemyType::Flagship][3]->setEnable(false);
    enemyShipsByTypes[EnemyType::Flagship][5]->setEnable(false);

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
    const auto director = Director::getInstance();

    if (director && director->getRunningScene() && director->getRunningScene()->isRunning()
        && director->getRunningScene()->getChildByName<MainScene*>(MAIN_SCENE_NAME)->getGameplayLayer()
        && director->getRunningScene()->getChildByName<MainScene*>(MAIN_SCENE_NAME)->getGameplayLayer()->isRunning())
    {
        for (const auto item: respawningCurrentlyMovingShips)
        {
            item->stopAllActions();
            item->launch();
        }
    }

    Node::onExit();
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
        nextInterval = random<float>(0.5f, 5.f);
        currentInterval = 0.f;

        const auto countOfEnabledFlagships = std::count_if(enemyShipsByTypes[EnemyType::Flagship].begin(),
                                                           enemyShipsByTypes[EnemyType::Flagship].end(),
                                                           [](const EnemyShip* enemyShip){return enemyShip->isEnable(); });

        decltype(enemyShips) enabledEnemyShips;
        std::copy_if(enemyShips.begin(), enemyShips.end(), std::back_inserter(enabledEnemyShips),
                     [countOfEnabledFlagships](const EnemyShip* enemyShip)
                     {
                         return enemyShip->isEnable() && (countOfEnabledFlagships ? enemyShip->getEnemyType() != EnemyType::Red : true);
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

        if(ship->getEnemyType() == EnemyType::Flagship)
        {
            flagshipLaunch(ship);
        }
        else
        {
            baseLaunch(ship);
        }
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
    enemyShipsByTypes.try_emplace(enemyType, EnemyVector{});
    enemyShipsByTypes[enemyType].push_back(enemyShip);

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

EnemyShip* EnemyFactory::spawnGameplayCopyOfEnemy(EnemyShip* toCopy) const
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
    currentCountOfRespawns += 1;
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    decltype(enemyShips) disabledShips;
    std::copy_if(enemyShips.begin(), enemyShips.end(), std::back_inserter(disabledShips),
                 [this](const EnemyShip* enemyShip)
                 {
                     if(enemyShip->getEnemyType() == EnemyType::Flagship)
                         return !enemyShip->isEnable()
                                && respawningCurrentlyFixedShips.find(enemyShip) == respawningCurrentlyFixedShips.end()
                                && respawnsNeededForFlagshipRespawn < currentCountOfRespawns;

                     return !enemyShip->isEnable()
                        && respawningCurrentlyFixedShips.find(enemyShip) == respawningCurrentlyFixedShips.end();
                 });

    if(disabledShips.empty())
        return;

    const auto toRespawn = disabledShips[random<unsigned int>(0, disabledShips.size() - 1)];
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

EnemyShip* EnemyFactory::baseLaunch(EnemyShip* enemyShip, float dir /*= 0.f*/)
{
    auto toLaunch = this->spawnGameplayCopyOfEnemy(enemyShip);
    toLaunch->launch(dir);
    enemyShip->setEnable(false);

    return toLaunch;
}

void EnemyFactory::flagshipLaunch(EnemyShip* enemyShip)
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

    const auto dir = random<int>(0, 1) ? 1.f : -1.f;
    const auto toLaunchFlagship = this->spawnGameplayCopyOfEnemy(enemyShip);
    toLaunchFlagship->launch(playerShip->getPosition(), dir);
    enemyShip->setEnable(false);

    const auto& flagships = enemyShipsByTypes[EnemyType::Flagship];
    const auto& redships = enemyShipsByTypes[EnemyType::Red];

    const auto index = std::find(flagships.begin(), flagships.end(), enemyShip) - flagships.begin();

    int pos = -1;
    for(int i = CLAMP<int>(index - 1, 0, redships.size() - 1); i <= CLAMP<int>(index + 1, 0, redships.size() - 1); ++i, ++pos)
    {
        if(!redships[i]->isEnable())
            continue;
        const auto toLaunch = this->spawnGameplayCopyOfEnemy(redships[i]);
        toLaunch->launch(playerShip->getPosition() + (toLaunch->getPosition() - toLaunchFlagship->getPosition()), dir);
        redships[i]->setEnable(false);
    }
}



