#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class HelloWorld : public cocos2d::Layer
{
public:
    cocos2d::Sprite *backgroundSprite, * panelSprite, *heroSprite, *itemSprite, *trophySprite, *timerSprite;
    int lives;
    std::vector<cocos2d::Sprite*> lifeSprites;
    cocos2d::FiniteTimeAction* idleBlinkingAction, *walkingAction, *hurtAction, *smokeAction;
    cocos2d::Vec2 playableRangeX, playableRangeY;
    cocos2d::Label* scoreLabel, *timerLabel, *playerNameLabel;
    cocos2d::ui::Button* startGameButton, *playAgainButton;
    cocos2d::ui::TextField* textField;
    
    bool isGameStart = false;
    bool isHurt = false, isDisappering = false;
    bool isGoingRight = false, isGoingLeft = false, isGoingUp = false, isGoingDown = false;
    int score = 0;
    float secondsLeftTimer = 31;
    
    cocos2d::Sprite* createSprite(float positionX, float positionY, float scaleCoef, const std::string &filename);
    void initializeMenuGameState();
    void startGame();
    void finishGame();
    void hurtStart();
    void hurtFinished();
    void changeItemZOrder();
    bool checkIfColisionWithBorders();
    bool checkIfPickedUpItem();
    void move();
    void generateItem();
    int getCountOfCurrentMoves();
    void updateScore();
    
    void update(float dt);
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void startGameClick(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
    void textFieldEvent(cocos2d::Ref* sender, cocos2d::ui::TextField::EventType type);
    
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
