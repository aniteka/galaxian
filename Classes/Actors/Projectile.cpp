#include <iostream>
#include "Projectile.h"
#include "Utilities.h"

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
}
