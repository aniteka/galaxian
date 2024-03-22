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

    void setEnable(bool inIsEnable);
    bool isEnable() const { return _isEnable; };

    EnemyType getEnemyType() const { return enemyType; }
    float getScore() const { return givenScore; }

    /*
     * 0.f == random
     * value should be 1.f, -1.f or 0.f
     */
    void launch(float dir = 0.f);
    void launch(cocos2d::Vec2 location, float dir = 0.f);

    void receiveDamage();

protected:
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    void rotateToPlayerShipUpdate(float delta);
    void shootingUpdate(float delta);
    void collisionUpdate(float delta);

    void rotateToPoint(cocos2d::Point point);

protected:
    EnemyType enemyType = EnemyType::Blue;
    bool isLaunched = false;
    bool _isEnable = true;

    float shootingTimeout = 1.5f;
    float currentShootingTimeout = 0.f;

    float givenScore = 0.f;

    static int flyingSoundId;
    static EnemyShip* currentFlyingShipUsedSound;
};

#endif //GALAXIAN_ENEMYSHIP_H
