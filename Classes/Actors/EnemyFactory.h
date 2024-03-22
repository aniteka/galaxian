#ifndef GALAXIAN_ENEMYFACTORY_H
#define GALAXIAN_ENEMYFACTORY_H

#include "cocos2d.h"
#include "EnemyShip.h"

class EnemyFactory : public cocos2d::Node
{
    using EnemyVector = std::vector<EnemyShip*>;

public:
    bool init() override;
    void update(float delta) override;
    void onExit() override;

    const std::vector<EnemyShip*>& getEnemyShipsArray() const { return enemyShips; };
    std::vector<EnemyShip*>& getEnemyShipsArray() { return enemyShips; };

    CREATE_FUNC(EnemyFactory);

protected:
    void moveUpdate(float interval);
    void launchRandomShipUpdate(float interval);

    EnemyShip* spawnEnemyShip(EnemyType enemyType);
    void spawnEnemyRow(int count, const float y, EnemyType enemyType);

    EnemyShip* spawnGameplayCopyOfEnemy(EnemyShip* toCopy) const;

    void respawnShipInIntervalCallback(float interval);

    EnemyShip* baseLaunch(EnemyShip* enemyShip, float dir = 0.f);
    void flagshipLaunch(EnemyShip* enemyShip);

protected:
    const std::string moveUpdateKey = "moveUpdate";
    bool movingRight = true;
    float movingDirMul = 1.f;
    float movingSpeed = 0.3f;
    float movingDownVal = 15.f;

    float respawnInterval = 4.f;
    int countOfRespawns = 15;
    int currentCountOfRespawns = 0;
    int respawnsNeededForFlagshipRespawn = 6;
    std::set<const EnemyShip*> respawningCurrentlyFixedShips;
    std::set<EnemyShip*> respawningCurrentlyMovingShips;

    float offsetMul = 0.09f;

    EnemyVector enemyShips;
    std::map<EnemyType, EnemyVector> enemyShipsByTypes;

private:
    float nextInterval = 5.f;
    float currentInterval = 0.f;
};


#endif //GALAXIAN_ENEMYFACTORY_H
