#ifndef GALAXIAN_ENEMYSHIP_H
#define GALAXIAN_ENEMYSHIP_H

#include <cocos2d.h>

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

protected:
    EnemyType enemyType = EnemyType::Blue;
};


#endif //GALAXIAN_ENEMYSHIP_H
