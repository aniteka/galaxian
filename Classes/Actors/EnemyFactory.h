#ifndef GALAXIAN_ENEMYFACTORY_H
#define GALAXIAN_ENEMYFACTORY_H

#include "cocos2d.h"
#include "EnemyShip.h"

class EnemyFactory : public cocos2d::Node
{
public:
    bool init() override;
    void update(float delta) override;

    const std::vector<EnemyShip*>& getEnemyShipsArray() const { return enemyShips; };
    std::vector<EnemyShip*>& getEnemyShipsArray() { return enemyShips; };

    CREATE_FUNC(EnemyFactory);

protected:
    void moveUpdate(float interval);

    EnemyShip* spawnEnemyShip(EnemyType enemyType);
    void spawnEnemyRow(int count, const float y, EnemyType enemyType);

    EnemyShip* spawnCopyOfEnemy(EnemyShip* toCopy) const;

protected:
    const std::string moveUpdateKey = "moveUpdate";
    bool movingRight = true;
    float movingDirMul = 1.f;
    float movingSpeed = 0.3f;
    float movingDownVal = 15.f;

    std::vector<EnemyShip*> enemyShips;
};


#endif //GALAXIAN_ENEMYFACTORY_H
