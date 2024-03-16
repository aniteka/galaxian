#ifndef GALAXIAN_ENEMYSHIP_H
#define GALAXIAN_ENEMYSHIP_H

#include <cocos2d.h>

class PlayerShip;

#define ENEMY_SHIP_TAG 2

enum class EnemyType
{
    Blue = 1, Purple, Red, Flagship,
    None = 0
};

class EnemyShip : public cocos2d::Sprite
{
public:
    static EnemyShip* createEnemyShip(EnemyType inEnemyType);

    bool init() override;
    void update(float delta) override;

    float getVisibleSizeWidth() const;
    float getVisibleSizeHeight() const;

    EnemyType getEnemyType() const { return enemyType; }

    void launch();

protected:
    void rotateToPlayerShipUpdate(float delta);

    void rotateToPoint(cocos2d::Point point);

protected:
    EnemyType enemyType = EnemyType::Blue;
    bool isLaunched = false;

private:
    PlayerShip* playerShip;
};


#endif //GALAXIAN_ENEMYSHIP_H
