#include "EnemyFactory.h"
#include "EnemyShip.h"
#include "Utilities.h"
#include "MainScene.h"
#include <iostream>

USING_NS_CC;

EnemyFactory *EnemyFactory::createEnemyFactory()
{
    return EnemyFactory::create();
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
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return false;
    }

    for (int i = 0; i < 3; ++i)
    {
        this->spawnEnemyRow(10, -45.f * (float)i, EnemyType::Blue);
    }
    this->spawnEnemyRow(8, 45.f, EnemyType::Purple);
    this->spawnEnemyRow(6, 45.f * 2.f, EnemyType::Red);

    this->scheduleUpdate();
    return true;
}

void EnemyFactory::update(float delta)
{
    Node::update(delta);

    moveUpdate(delta);
}

void EnemyFactory::moveUpdate(float interval)
{
    const auto director = Director::getInstance();
    if (!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }

    if (getPositionX() >= (director->getVisibleSize().width / 2.f) + 64.f
        || getPositionX() <= (director->getVisibleSize().width / 2.f) - 64.f)
    {
        const auto toLaunch = spawnCopyOfEnemy(enemyShips[0]);
        toLaunch->launch();

        movingRight = !movingRight;
        if (movingRight)
            movingDirMul = 1.f;
        else if (!movingRight)
            movingDirMul = -1.f;

        this->setPositionY(this->getPositionY() - movingDownVal);
    }
    this->setPositionX(this->getPositionX() + movingSpeed * movingDirMul);
}

EnemyShip* EnemyFactory::spawnEnemyShip(EnemyType enemyType)
{
    const auto enemyShip = EnemyShip::createEnemyShip(enemyType);
    if (!enemyShip)
    {
        std::cout << GENERATE_ERROR_MESSAGE(enemyShip);
        return nullptr;
    }
    this->addChild(enemyShip);
    enemyShips.push_back(enemyShip);

    // TODO DEBUG ONLY
    enemyShip->setScale(0.4f);
    // TODO DEBUG ONLY

    return enemyShip;
}

void EnemyFactory::spawnEnemyRow(int count, const float y, EnemyType enemyType)
{
    const auto director = Director::getInstance();
    if (!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }

    for (int j = 0; j < count; ++j)
    {
        const auto ship = this->spawnEnemyShip(enemyType);
        if(!ship)
        {
            std::cout << GENERATE_ERROR_MESSAGE(ship);
            continue;
        }

        const auto wWidth = director->getVisibleSize().width;
        const auto offset = wWidth * 0.08f;
        const auto shipWidth = ship->getVisibleSizeWidth();

        const auto x =
                ((offset * (float)j)
                 - (wWidth / 2.f))
                + ((wWidth - ( shipWidth*(float)count + (offset - shipWidth)*((float)(count - 1)) ) ) / 2.f + shipWidth / 2.f);

        ship->setPosition(x, y);
    }
}

EnemyShip *EnemyFactory::spawnCopyOfEnemy(EnemyShip *toCopy) const
{
    const auto mainScene = getRunningScene<MainScene*>();
    if(!mainScene)
    {
        std::cout << GENERATE_ERROR_MESSAGE(mainScene);
        return nullptr;
    }

    const auto enemyShip = EnemyShip::createEnemyShip(toCopy->getEnemyType());
    if (!enemyShip)
    {
        std::cout << GENERATE_ERROR_MESSAGE(enemyShip);
        return nullptr;
    }
    mainScene->addChild(enemyShip);

    enemyShip->setPosition(this->convertToWorldSpace(toCopy->getPosition()));

    // TODO DEBUG ONLY
    enemyShip->setScale(0.4f);
    // TODO DEBUG ONLY

    return enemyShip;
}
