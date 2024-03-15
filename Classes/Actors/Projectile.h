#ifndef GALAXIAN_PROJECTILE_H
#define GALAXIAN_PROJECTILE_H

#include <cocos2d.h>

class Projectile : public cocos2d::Sprite
{
public:
    static Projectile* createProjectile();

    bool init() override;
    void update(float delta) override;

    void launch();

    float getVisibleSizeWidth() const;
    float getVisibleSizeHeight() const;

    CREATE_FUNC(Projectile);

    std::function<void(cocos2d::Sprite*/*hit sprite(nullable)*/)> onHit;

protected:
    void collisionUpdate(float delta);

    float speed = 6.f;

private:
    bool hasLaunched = false;
};


#endif //GALAXIAN_PROJECTILE_H
