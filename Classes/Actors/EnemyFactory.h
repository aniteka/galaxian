#ifndef GALAXIAN_ENEMYFACTORY_H
#define GALAXIAN_ENEMYFACTORY_H

#include "cocos2d.h"

class EnemyFactory : cocos2d::Node
{
public:
    static EnemyFactory* createEnemyFactory();

    bool init() override;
    void update(float delta) override;

    CREATE_FUNC(EnemyFactory);

protected:
    
};


#endif //GALAXIAN_ENEMYFACTORY_H
