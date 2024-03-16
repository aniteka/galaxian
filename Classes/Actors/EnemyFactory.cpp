#include "EnemyFactory.h"
#include "EnemyShip.h"
#include "Utilities.h"
#include <iostream>

USING_NS_CC;

EnemyFactory* EnemyFactory::createEnemyFactory()
{
    return EnemyFactory::create();
}

bool EnemyFactory::init()
{
    if(!Node::init())
    {
        return false;
    }
    const auto director = Director::getInstance();
    if(!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return false;
    }

    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 10; ++j)
        {
            auto enemyShip = EnemyShip::createEnemyShip(EnemyType::Blue);
            if(!enemyShip)
            {
                std::cout << GENERATE_ERROR_MESSAGE(enemyShip);
                continue;
            }
            this->addChild(enemyShip);
            enemyShips.push_back(enemyShip);

            const auto x =
                    ((director->getVisibleSize().width * 0.08f * (float)j)
                    - (director->getVisibleSize().width / 2.f))
                    + 85.f;
            const auto y = -45.f * (float)i;

            enemyShip->setPosition(x, y);

            // TODO DEBUG ONLY
            enemyShip->setScale(0.4f);
            // TODO DEBUG ONLY
        }
    }

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
    if(!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }

    if(getPositionX() >= (director->getVisibleSize().width / 2.f) + 62.5f
        || getPositionX() <= (director->getVisibleSize().width / 2.f) - 62.5f)
    {
        movingRight = !movingRight;
        if(movingRight)
            movingDirMul = 1.f;
        else if(!movingRight)
            movingDirMul = -1.f;

        this->setPositionY(this->getPositionY() - movingDownVal);
    }
    this->setPositionX(this->getPositionX() + movingSpeed * movingDirMul);
}
