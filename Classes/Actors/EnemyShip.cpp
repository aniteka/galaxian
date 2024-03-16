#include <iostream>
#include "EnemyShip.h"
#include "Utilities.h"

EnemyShip* EnemyShip::createEnemyShip(EnemyType inEnemyType)
{
    auto pRet = new(std::nothrow)EnemyShip();
    if(pRet)
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
    switch(enemyType)
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
    case EnemyType::None: default:
        std::cout << GENERATE_ERROR_MESSAGE(enemyType);
        return false;
    }

    if(!Sprite::initWithFile(enemyShipFile))
    {
        return false;
    }

    return true;
}

void EnemyShip::update(float delta)
{
    Node::update(delta);

}

float EnemyShip::getVisibleSizeWidth() const
{
    return this->getContentSize().width * this->getScaleX();
}

float EnemyShip::getVisibleSizeHeight() const
{
    return this->getContentSize().height * this->getScaleY();
}
