#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    backgroundSprite = createSprite(0, 0, 1, "ground.png");
    backgroundSprite -> setAnchorPoint(Vec2(0,0));
    this -> addChild(backgroundSprite);
    
    panelSprite = createSprite(backgroundSprite -> getContentSize().width/2, Director::getInstance() -> getWinSize().height - 57 , 1, "panel.png");
    this -> addChild(panelSprite);
    
    lives = 3;

    for (int i = 0; i < lives; ++i) {
        Sprite* lifeSprite = Sprite::create("heart.png");
        lifeSprite->setScale(0.8);
        lifeSprite->setAnchorPoint(Vec2(0.5, 0.5));
        lifeSprite->setPosition(Vec2(40 + i * 60, panelSprite->getPosition().y - panelSprite->getContentSize().height / 2 - 40));
        this->addChild(lifeSprite);
        lifeSprites.push_back(lifeSprite);
    }
    
    itemSprite = createSprite(0, 0, 2, "skull.png");
    itemSprite -> setVisible(false);
    this -> addChild(itemSprite);
    
    heroSprite = createSprite(Director::getInstance() -> getWinSize().width / 2, Director::getInstance() -> getWinSize().height / 2, 0.3, "Hero/Idle Blinking/Idle Blinking_000.png");
    this -> addChild(heroSprite, 2);
    
    trophySprite = createSprite(50, Director::getInstance() -> getWinSize().height - 55, 1.5, "GoldTrophy.png");
    this -> addChild(trophySprite);
    
    playerNameLabel = Label::createWithSystemFont("", "Arial", 24);
        playerNameLabel->setPosition(Vec2(trophySprite->getPosition().x + trophySprite->getContentSize().width / 2 * trophySprite->getScaleX() + 15, panelSprite->getPosition().y - 40));
        this->addChild(playerNameLabel);
    
    timerSprite = createSprite(panelSprite -> getContentSize().width - 130, panelSprite -> getPosition().y, 1, "timerButton.png");
    this -> addChild(timerSprite);
    
    auto idleBlinkingAnimation = Animation::create();
    for (int i = 1; i <= 11; i++) {
        std::string name = StringUtils::format("Hero/Idle Blinking/Idle Blinking_%003d.png", i);
        idleBlinkingAnimation -> addSpriteFrameWithFile(name.c_str());
    }
    idleBlinkingAnimation -> setDelayPerUnit(0.1f);
    idleBlinkingAnimation -> setLoops(-1);
    
    idleBlinkingAction = Animate::create(idleBlinkingAnimation);
    
    
    auto walkingAnimation = Animation::create();
    
    for (int i = 1; i <= 11; i++) {
        std::string name = StringUtils::format("Hero/Walking/Walking_%003d.png", i);
        walkingAnimation -> addSpriteFrameWithFile(name.c_str());
    }
    walkingAnimation -> setDelayPerUnit(0.04f);
    walkingAnimation -> setLoops(-1);
    
    walkingAction = Animate::create(walkingAnimation);
    
    
    auto hurtAnimation = Animation::create();
    for (int i = 1; i <= 11; i++) {
        std::string name = StringUtils::format("Hero/Hurt/Hurt_%003d.png", i);
        hurtAnimation -> addSpriteFrameWithFile(name.c_str());
    }
    hurtAnimation -> setDelayPerUnit(0.1f);
    hurtAnimation -> setLoops(2);
    
    hurtAction = Animate::create(hurtAnimation);
    
    
    auto smokeAnimation = Animation::create();
    for (int i = 1; i <= 9; i++) {
        std::string name = StringUtils::format("Hero/Smoke/Smoke_%003d.png", i);
        smokeAnimation -> addSpriteFrameWithFile(name.c_str());
    }
    smokeAnimation -> setDelayPerUnit(0.01f);
    smokeAction = Animate::create(smokeAnimation);
    
    playableRangeX = Vec2(
                          itemSprite -> getContentSize().width * itemSprite -> getScaleX(),
                          Director::getInstance() -> getWinSize().width - itemSprite -> getContentSize().width * itemSprite -> getScaleX()
                          );
    
    playableRangeY = Vec2(
                          itemSprite -> getContentSize().height * itemSprite -> getScaleX(),
                          panelSprite -> getPosition().y - panelSprite -> getContentSize().height/2 - itemSprite -> getContentSize().height * itemSprite -> getScaleX()
                          );
    
    scoreLabel = Label::createWithSystemFont("Score", "Arial Black", 72);
    scoreLabel -> setAnchorPoint(Vec2(0, 0.5));
    scoreLabel -> setColor(Color3B(255, 204, 0));
    scoreLabel -> setPosition(Vec2(trophySprite -> getPosition().x + trophySprite -> getContentSize().width / 2 * trophySprite -> getScaleX() + 15, panelSprite -> getPosition().y));
    this -> addChild(scoreLabel);
    
    timerLabel = Label::createWithSystemFont("Timer", "Arial Black", 46);
    timerLabel -> setColor(Color3B(255, 204, 0));
    timerLabel -> setPosition(Vec2(timerSprite -> getPosition().x, timerSprite -> getPosition().y));
    this -> addChild(timerLabel);
    
    startGameButton = ui::Button::create("StartBtn.png", "StartBtn.png", "StartBtn.png");
    startGameButton -> setPosition(Vec2(panelSprite -> getPosition().x, panelSprite -> getPosition().y));
    startGameButton -> addTouchEventListener(CC_CALLBACK_2(HelloWorld::startGameClick, this));
    this -> addChild(startGameButton);
    
    playAgainButton = ui::Button::create("PlayAgainBtn.png", "PlayAgainBtn.png", "PlayAgainBtn.png");
    playAgainButton -> setPosition(Vec2(panelSprite -> getPosition().x, panelSprite -> getPosition().y));
    playAgainButton -> addTouchEventListener(CC_CALLBACK_2(HelloWorld::startGameClick, this));
    playAgainButton -> setVisible(false);
    this -> addChild(playAgainButton);
    
    textField = ui::TextField::create("Enter your name", "Arial", 30);
    textField->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, heroSprite->getPosition().y-250));
    textField->addEventListener(CC_CALLBACK_2(HelloWorld::textFieldEvent, this));
    this->addChild(textField);
    textField->setVisible(true);

    playerNameLabel = Label::createWithSystemFont("", "Arial", 24);
    playerNameLabel->setPosition(Vec2(startGameButton->getPosition().x, startGameButton->getPosition().y));
    this->addChild(playerNameLabel);
    playerNameLabel->setVisible(false);
    
    initializeMenuGameState();
    
    auto listener = EventListenerKeyboard::create();
    listener -> onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
    listener -> onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    
    _eventDispatcher -> addEventListenerWithSceneGraphPriority(listener, this);
    
    idleBlinkingAction -> retain();
    walkingAction -> retain();
    hurtAction -> retain();
    smokeAction -> retain();
    
    this->scheduleUpdate();
    
    return true;
}

void HelloWorld::textFieldEvent(Ref* sender, ui::TextField::EventType type) {
    switch (type) {
    case ui::TextField::EventType::ATTACH_WITH_IME:
        CCLOG("Displayed keyboard");
        break;
    case ui::TextField::EventType::DETACH_WITH_IME:
        CCLOG("Dismissed keyboard");
        break;
    case ui::TextField::EventType::INSERT_TEXT:
        CCLOG("Inserted text: %s", textField->getString().c_str());
        break;
    case ui::TextField::EventType::DELETE_BACKWARD:
        CCLOG("Deleted backward");
        break;
    default:
        break;
    }
}



Sprite* HelloWorld::createSprite(float positionX, float positionY, float scaleCoef, const std::string &filename) {
    auto sprite = Sprite::create(filename);
    sprite -> setPosition(Vec2(positionX, positionY));
    sprite -> setScale(scaleCoef);
    return sprite;
}

void HelloWorld::initializeMenuGameState() {
    lives = 3;

    for (auto lifeSprite : lifeSprites) {
        lifeSprite->setVisible(true);
    }
    
    isGameStart = false;
    heroSprite -> stopAllActions();
    isGoingRight = false;
    isGoingLeft = false;
    isGoingUp = false;
    isGoingDown = false;
    
    itemSprite -> setVisible(false);
    
    heroSprite -> setScale(1);
    heroSprite -> setPosition(Vec2(Director::getInstance() -> getWinSize().width / 2, Director::getInstance() -> getWinSize().height / 2));
    heroSprite -> runAction(idleBlinkingAction);
}

void HelloWorld::startGame() {
    std::string playerName = textField->getString();
    playerNameLabel->setString(playerName);
    
    // Hide the text field when the game starts
    textField->setVisible(false);

    // Show the player name label when the game starts
    playerNameLabel->setVisible(true);
    
    startGameButton -> setVisible(false);
    playAgainButton -> setVisible(false);
    
    score = 0;
    secondsLeftTimer = 31;
    
    heroSprite -> setScale(0.4);
    scoreLabel -> setString("0");
    timerLabel -> setString(std::to_string(secondsLeftTimer));
    
    isGameStart = true;
    generateItem();
}

void HelloWorld::finishGame() {
    textField->setVisible(true);

    // Hide the player name label when the game finishes
    playerNameLabel->setVisible(false);
    initializeMenuGameState();
    timerLabel -> setString("Timer");
    playAgainButton -> setVisible(true);
}

void HelloWorld::hurtStart() {
    heroSprite -> stopAllActions();
    
    isGoingRight = false;
    isGoingLeft = false;
    isGoingUp = false;
    isGoingDown = false;
    
    heroSprite -> runAction(Sequence::create(hurtAction ,CallFunc::create(CC_CALLBACK_0(HelloWorld::hurtFinished, this)), NULL));
    
    lives--;
    if (lives >= 0) {
        lifeSprites[lives]->setVisible(false);
    }

    if (lives <= 0) {
        finishGame();
    }
}

void HelloWorld::hurtFinished() {
    isHurt = false;
    heroSprite -> setTexture("Idle Blinking_000.png");
}

void HelloWorld::changeItemZOrder() {
    itemSprite -> setLocalZOrder(100);
}

bool HelloWorld::checkIfColisionWithBorders() {
    if (heroSprite -> getPosition().y + heroSprite -> getContentSize().height/2 * abs(heroSprite -> getScaleX())-15 > (panelSprite -> getPosition().y - panelSprite -> getContentSize().height/2)) {
        isHurt = true;
        heroSprite -> setPositionY(heroSprite -> getPosition().y - 20);
    }
    
    if (heroSprite -> getPosition().y - heroSprite -> getContentSize().height/2 * abs(heroSprite -> getScaleX()/2) < 0) {
        isHurt = true;
        heroSprite -> setPositionY(heroSprite -> getPosition().y + 20);
    }
    if (heroSprite -> getPosition().x + heroSprite -> getContentSize().width/2 * abs(heroSprite -> getScaleX()/2) > Director::getInstance() -> getWinSize().width) {
        isHurt = true;
        heroSprite -> setPositionX(heroSprite -> getPosition().x - 20);
    }
    
    if (heroSprite -> getPosition().x - heroSprite -> getContentSize().width/2 * abs(heroSprite -> getScaleX()/2) < 0) {
        isHurt = true;
        heroSprite -> setPositionX(heroSprite -> getPosition().x + 20);
    }
    
    return isHurt;
}

bool HelloWorld::checkIfPickedUpItem() {
    
    if ((heroSprite -> getPosition().x >= itemSprite -> getPosition().x - itemSprite -> getContentSize().width * itemSprite->getScaleX())
        && (heroSprite -> getPosition().x <= itemSprite -> getPosition().x + itemSprite -> getContentSize().width * itemSprite->getScaleX())
        && (heroSprite -> getPosition().y <= itemSprite -> getPosition().y + itemSprite -> getContentSize().height * itemSprite->getScaleX())
        && (heroSprite -> getPosition().y >= itemSprite -> getPosition().y - itemSprite -> getContentSize().height * itemSprite->getScaleX())) {
        return true;
    }
    
    return false;
}

void HelloWorld::move() {
    if (isGoingRight) {
        heroSprite -> setPosition(Vec2(heroSprite -> getPosition().x + 5, heroSprite -> getPosition().y));
    }
    
    if (isGoingLeft) {
        heroSprite -> setPosition(Vec2(heroSprite -> getPosition().x - 5, heroSprite -> getPosition().y));
    }
    
    if (isGoingUp) {
        heroSprite -> setPosition(Vec2(heroSprite -> getPosition().x, heroSprite -> getPosition().y + 5));
    }
    
    if (isGoingDown) {
        heroSprite -> setPosition(Vec2(heroSprite -> getPosition().x, heroSprite -> getPosition().y - 5));
    }
}

void HelloWorld::generateItem() {
    isDisappering = false;
    itemSprite -> setScale(2);
    itemSprite -> setLocalZOrder(1);
    itemSprite -> setTexture("skull.png");
    itemSprite -> setPosition(Vec2(
                                    rand() % ((int) (playableRangeX.y - playableRangeX.x + 1)) + playableRangeX.x,
                                    rand() % ((int) (playableRangeY.y - playableRangeY.x + 1)) + playableRangeY.x
                                    ));
    
    if (isGameStart) itemSprite -> setVisible(true);
}

int HelloWorld::getCountOfCurrentMoves() {
    int count = 0;
    
    if (isGoingRight) count++;
    if (isGoingLeft) count++;
    if (isGoingUp) count++;
    if (isGoingDown) count++;
    
    return count;
    
}

void HelloWorld::updateScore() {
    score++;
    scoreLabel -> setString(std::to_string(score));
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    
    if (!isGameStart || isHurt) return;
    
    heroSprite -> stopAllActions();
    
    switch ((int)keyCode) {
        case 27:
            isGoingRight = true;
            heroSprite -> setScaleX(abs(heroSprite -> getScaleX()));
            heroSprite -> runAction(walkingAction);
            break;
            
        case 26:
            isGoingLeft = true;
            heroSprite -> setScaleX(-abs(heroSprite -> getScaleX()));
            heroSprite -> runAction(walkingAction);
            break;
            
        case 28:
            isGoingUp = true;
            heroSprite -> runAction(walkingAction);
            break;
            
        case 29:
            isGoingDown = true;
            heroSprite -> runAction(walkingAction);
            break;
            
        default:
            break;
    }
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    if (!isGameStart) return;
    
    switch ((int)keyCode) {
        case 27:
            isGoingRight = false;
            break;
            
        case 26:
            isGoingLeft = false;
            break;
            
        case 28:
            isGoingUp = false;
            break;
            
        case 29:
            isGoingDown = false;
            break;
            
        default:
            break;
    }
    
    switch ((int)keyCode) {
        case 27:
        case 26:
        case 28:
        case 29:
            heroSprite -> stopAction(walkingAction);
            
            if (getCountOfCurrentMoves() > 0) {
                heroSprite -> stopAllActions();
                heroSprite -> runAction(walkingAction);
            }
            
            break;
            
        default:
            break;
    }
}

void HelloWorld::startGameClick(Ref* pSender, ui::Widget::TouchEventType type) {
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN:
            startGame();
            break;
            
        case ui::Widget::TouchEventType::MOVED:
            
            break;
      
        case ui::Widget::TouchEventType::ENDED:
            
            break;
            
        case ui::Widget::TouchEventType::CANCELED:
            
            break;
        default:
            break;
        }
}

void HelloWorld::update(float dt) {
    
    if (isGameStart) {
        secondsLeftTimer -= dt;
        timerLabel -> setString(std::to_string((int)secondsLeftTimer));
        
        if ((int) secondsLeftTimer <= 0) {
            finishGame();
            return;
        }
        
        if (!isHurt) {
            if (checkIfColisionWithBorders()) {
                hurtStart();
            } else {
                
                if (!isDisappering && checkIfPickedUpItem()) {
                    updateScore();
                    itemSprite -> stopAllActions();
                    itemSprite -> setScale(0.5);
                    itemSprite -> runAction(Sequence::create(CallFunc::create(CC_CALLBACK_0(HelloWorld::changeItemZOrder, this)), smokeAction ,CallFunc::create(CC_CALLBACK_0(HelloWorld::generateItem, this)), NULL));
                    isDisappering = true;
                }
                move();
            }
        }
    }
}

