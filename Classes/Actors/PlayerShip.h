#ifndef GALAXIAN_PLAYERSHIP_H
#define GALAXIAN_PLAYERSHIP_H

#include <cocos2d.h>
#include <Utilities.h>

class PlayerShip : public cocos2d::Sprite
{
public:
    static cocos2d::Sprite* createSprite();

    bool init() override;
    void update(float delta) override;

    CREATE_FUNC(PlayerShip);

protected:
    void setupBody();
    void setupKeyboard();

    void onKeyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

protected:
    cocos2d::Sprite* body = nullptr;

    cocos2d::EventListenerKeyboard* keyboardListener = nullptr;

    bool movingLeftState = false;
    bool movingRightState = false;

private:
    static const char* const BODY_SPRITE;
};


#endif //GALAXIAN_PLAYERSHIP_H
