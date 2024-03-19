#include "EnemyProjectile.h"
#include "PlayerShip.h"
#include "Utilities.h"

USING_NS_CC;
USING_NS_CC_MATH;

bool EnemyProjectile::init()
{
    if(!Sprite::initWithFile(PROJECTILE_SPRITE))
    {
        return false;
    }

    this->setTag(ENEMY_PROJECTILE_TAG);

    this->setScale(3.125f);

    this->setAnchorPoint(Vec2(0.5, 0));

    const auto physicsBody = PhysicsBody::createBox(this->getContentSize());
    physicsBody->setDynamic(false);
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    this->setPhysicsBody(physicsBody);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(EnemyProjectile::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    this->scheduleUpdate();
    return true;
}

void EnemyProjectile::update(float delta)
{
    Node::update(delta);

    collisionUpdate(delta);
}

void EnemyProjectile::launch(cocos2d::Vec2 pos)
{
    const auto dir = (this->getPosition() - pos).getNormalized();
    const auto finalPos = pos - dir * 200.f;
    const auto action = MoveTo::create((this->getPosition() - finalPos).getLength() * speedMul, finalPos);
    this->runAction(action);
}

bool EnemyProjectile::onContactBegin(PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if(nodeA && nodeB
       && (nodeA->getTag() == ENEMY_PROJECTILE_TAG || nodeB->getTag() == ENEMY_PROJECTILE_TAG)
       && (nodeA->getTag() == PLAYER_SHIP_TAG || nodeB->getTag() == PLAYER_SHIP_TAG))
    {
        const auto self = dynamic_cast<EnemyProjectile*>(nodeA->getTag() == ENEMY_PROJECTILE_TAG ? nodeA : nodeB);
        const auto ship = dynamic_cast<PlayerShip*>(nodeA->getTag() == PLAYER_SHIP_TAG ? nodeA : nodeB);
        if(!self || !ship)
        {
            CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(!self || !ship));
            return false;
        }

        self->removeFromParent();

        ship->receiveDamage();
    }

    return false;
}

void EnemyProjectile::collisionUpdate(float delta)
{
    const auto director = Director::getInstance();
    if (!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    if(this->getPosition().y <= -15.f
        || this->getPosition().y <= -15.f
        || this->getPosition().y >= director->getVisibleSize().width + 15.f)
    {
        this->removeFromParent();
    }
}
