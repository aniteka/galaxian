#include <iostream>
#include "Projectile.h"
#include "Utilities.h"
#include "MainScene.h"
#include "EnemyFactory.h"
#include "EnemyShip.h"

USING_NS_CC;

Projectile* Projectile::createProjectile()
{
    return Projectile::create();
}

bool Projectile::init()
{
    if(!Sprite::initWithFile(PROJECTILE_SPRITE))
    {
        return false;
    }

    // TODO DEBUG ONLY
    this->setScaleX(0.1);
    this->setScaleY(0.3);
    // TODO DEBUG ONLY

    this->setAnchorPoint(Vec2(0.5, 0));

    this->scheduleUpdate();
    return true;
}

void Projectile::update(float delta)
{
    Node::update(delta);

    if(hasLaunched)
    {
        setPosition(Vec2(getPosition().x, getPosition().y + speed));

        collisionUpdate(delta);
    }
}

float Projectile::getVisibleSizeWidth() const
{
    return this->getContentSize().width * this->getScaleX();
}

float Projectile::getVisibleSizeHeight() const
{
    return this->getContentSize().height * this->getScaleY();
}

void Projectile::launch()
{
    hasLaunched = true;
}

void Projectile::collisionUpdate(float delta)
{
    const auto director = Director::getInstance();
    if(!director)
    {
        std::cout << GENERATE_ERROR_MESSAGE(director);
        return;
    }

    const auto windowSize = director->getVisibleSize();

    if(windowSize.height < this->getPositionY())
    {
        onHit(nullptr);
        this->removeFromParent();
        return;
    }

    mainScene = mainScene ? mainScene : dynamic_cast<MainScene*>(director->getRunningScene());
    if(!mainScene)
    {
        std::cout << GENERATE_ERROR_MESSAGE(mainScene);
        return;
    }
    const auto enemyFactory = mainScene->getEnemyFactory();
    if(!enemyFactory)
    {
        std::cout << GENERATE_ERROR_MESSAGE(enemyFactory);
        return;
    }

    const auto& enemyShipArray = enemyFactory->getEnemyShipsArray();
    for (const auto& ship : enemyShipArray)
    {
        if(!ship)
        {
            std::cout << GENERATE_ERROR_MESSAGE(ship);
            continue;
        }

        if(!ship->isVisible()) continue;

        const auto posA = enemyFactory->convertToWorldSpace(ship->getPosition());
        const auto aLeft    = posA.x    - ship->getVisibleSizeWidth() / 2.f;
        const auto aRight   = posA.x    + ship->getVisibleSizeWidth() / 2.f;
        const auto aTop     = posA.y    + ship->getVisibleSizeHeight() / 2.f;
        const auto aBottom  = posA.y    - ship->getVisibleSizeHeight() / 2.f;

        const auto posB = this->getPosition();
        const auto bLeft    = posB.x    - this->getVisibleSizeWidth() / 2.f;
        const auto bRight   = posB.x    + this->getVisibleSizeWidth() / 2.f;
        const auto bTop     = posB.y    + this->getVisibleSizeHeight();
        const auto bBottom  = posB.y    - 0.f;

        if (aLeft < bRight && aRight > bLeft &&
            aTop > bBottom && aBottom < bTop )
        {
            ship->setVisible(false);

            onHit(nullptr);
            this->removeFromParent();
            return;
        }
    }

}
