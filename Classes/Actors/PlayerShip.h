#ifndef GALAXIAN_PLAYERSHIP_H
#define GALAXIAN_PLAYERSHIP_H

#include <cocos2d.h>

class PlayerShip : public cocos2d::Sprite
{
public:
    static cocos2d::Sprite* createSprite();

    virtual bool init();

    CREATE_FUNC(PlayerShip);

protected:
    void setupBody();
    void setupKeyboard();

    void onKeyPressedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleasedCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

protected:
    cocos2d::Sprite* body = nullptr;

    cocos2d::EventListenerKeyboard* keyboardListener = nullptr;

private:
    static const char* const BODY_SPRITE;
};


#endif //GALAXIAN_PLAYERSHIP_H
