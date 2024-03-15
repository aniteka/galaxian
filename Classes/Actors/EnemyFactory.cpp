#include "EnemyFactory.h"

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

    return true;
}

void EnemyFactory::update(float delta)
{
    Node::update(delta);


}
