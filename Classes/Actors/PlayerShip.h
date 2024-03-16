#ifndef GALAXIAN_PLAYERSHIP_H
#define GALAXIAN_PLAYERSHIP_H

#include <cocos2d.h>
#include <Utilities.h>

class Projectile;

class PlayerShip : public cocos2d::Sprite
{
public:
    static PlayerShip* createPlayerShip();

    bool init() override;
    void update(float delta) override;

    float getVisibleBodySizeWidth() const;
    float getVisibleBodySizeHeight() const;

    CREATE_FUNC(PlayerShip);

protected:
    void setupBody();
    void setupProjectileView();
    void setupKeyboard();

    void movingUpdate();

    void shoot();

    void onProjectileHit(cocos2d::Sprite* hitSprite);
    void onKeyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

protected:
    cocos2d::Sprite* body = nullptr;
    Projectile* projectileView = nullptr;
    Projectile* projectileReal = nullptr;

    cocos2d::EventListenerKeyboard* keyboardListener = nullptr;

    float speed = 5.f;

    bool movingLeftState = false;
    bool movingRightState = false;
};


#endif //GALAXIAN_PLAYERSHIP_H
