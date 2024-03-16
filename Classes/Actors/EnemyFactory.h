#ifndef GALAXIAN_ENEMYFACTORY_H
#define GALAXIAN_ENEMYFACTORY_H

#include "cocos2d.h"

class EnemyShip;

class EnemyFactory : public cocos2d::Node
{
public:
    static EnemyFactory* createEnemyFactory();

    bool init() override;
    void update(float delta) override;

    const std::vector<EnemyShip*>& getEnemyShipsArray() const { return enemyShips; };
    std::vector<EnemyShip*>& getEnemyShipsArray() { return enemyShips; };

    CREATE_FUNC(EnemyFactory);

protected:
    void moveUpdate(float interval);

protected:
    const std::string moveUpdateKey = "moveUpdate";
    bool movingRight = true;
    float movingDirMul = 1.f;
    float movingSpeed = 0.3f;
    float movingDownVal = 15.f;

    std::vector<EnemyShip*> enemyShips;
};


#endif //GALAXIAN_ENEMYFACTORY_H
