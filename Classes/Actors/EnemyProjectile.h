#ifndef GALAXIAN_ENEMYPROJECTILE_H
#define GALAXIAN_ENEMYPROJECTILE_H

#include <cocos2d.h>

#define ENEMY_PROJECTILE_TAG 3

class EnemyProjectile : public cocos2d::Sprite
{
public:
    bool init() override;

    void update(float delta) override;

    void launch(cocos2d::Vec2 pos);

    CREATE_FUNC(EnemyProjectile);
protected:
    bool onContactBegin(cocos2d::PhysicsContact& contact);

    void collisionUpdate(float delta);

    float speedMul = 0.004f;
};


#endif //GALAXIAN_ENEMYPROJECTILE_H
