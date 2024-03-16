#include <iostream>
#include "Projectile.h"
#include "Utilities.h"
#include "MainScene.h"
#include "EnemyFactory.h"
#include "EnemyShip.h"

USING_NS_CC;

bool Projectile::init()
{
    if(!Sprite::initWithFile(PROJECTILE_SPRITE))
    {
        return false;
    }

    this->setTag(PROJECTILE_TAG);

    // TODO DEBUG ONLY
    this->setScaleX(0.1);
    this->setScaleY(0.3);
    // TODO DEBUG ONLY

    this->setAnchorPoint(Vec2(0.5, 0));

    const auto physicsBody = PhysicsBody::createBox(Size(
            this->getVisibleSizeWidth() * 10,
            this->getVisibleSizeHeight() * 3));
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    physicsBody->setEnabled(false);
    this->setPhysicsBody(physicsBody);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Projectile::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

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
    this->getPhysicsBody()->setEnabled(true);
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

bool Projectile::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if(nodeA && nodeB
        && (nodeA->getTag() == PROJECTILE_TAG || nodeB->getTag() == PROJECTILE_TAG)
        && (nodeA->getTag() == ENEMY_SHIP_TAG || nodeB->getTag() == ENEMY_SHIP_TAG))
    {
        const auto self = dynamic_cast<Projectile*>(nodeA->getTag() == PROJECTILE_TAG ? nodeA : nodeB);
        const auto ship = dynamic_cast<EnemyShip*>(nodeA->getTag() == ENEMY_SHIP_TAG ? nodeA : nodeB);
        if(!self || !ship)
        {
            std::cout << GENERATE_ERROR_MESSAGE(!self || !ship);
            return false;
        }

        ship->setEnable(false);

        self->onHit(ship);
        self->removeFromParent();
    }

    return false;
}
