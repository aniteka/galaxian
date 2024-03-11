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
    cocos2d::Sprite* body = nullptr;
};


#endif //GALAXIAN_PLAYERSHIP_H
