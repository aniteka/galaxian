#include <iostream>
#include "MainScene.h"
#include "Actors/PlayerShip.h"
#include "Actors/EnemyFactory.h"
#include "Utilities.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    const auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = MainScene::create();
    scene->addChild(layer, 0, MAIN_SCENE_NAME);
    return scene;
}

bool MainScene::init()
{
    if (!Layer::init())
    {
        return false;
    }

    setupMainMenu();
    //setupGameplay();

    return true;
}

void MainScene::updateGameplayMenu()
{
    if(maxScore < playerShip->getScore())
        maxScore = playerShip->getScore();

    scoreLabel->setString(StringUtils::format("Score: %.0f", playerShip->getScore()));
    hpLabel->setString(StringUtils::format("Hp: %i", playerShip->getHp()));
}

void MainScene::setupMainMenu()
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    mainMenu = Layer::create();
    if(!mainMenu)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(mainMenu));
        return;
    }
    this->addChild(mainMenu, 1);

    const auto bg = Sprite::create("Background.png");
    if (bg)
    {
        mainMenu->addChild(bg, 0);
        bg->setAnchorPoint(Vec2(0, 0));
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(bg));
    }

    const auto buttonStart = ui::Button::create(START_BUTTON_SPRITE,START_BUTTON_SPRITE,START_BUTTON_SPRITE);
    if(buttonStart)
    {
        buttonStart->setPosition(
                Vec2(director->getVisibleSize().width / 2.f, director->getVisibleSize().height / 2.f + 40.f));
        buttonStart->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type)
                                           {
                                               if (type == ui::Widget::TouchEventType::ENDED)
                                               {
                                                   setupGameplay();
                                                   releaseMainMenu();
                                               }
                                           });
        mainMenu->addChild(buttonStart);
    }

    const auto buttonExit = ui::Button::create(EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE);
    if(buttonExit)
    {
        buttonExit->setPosition(
                Vec2(director->getVisibleSize().width / 2.f, director->getVisibleSize().height / 2.f - 40.f));
        buttonExit->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type)
                                          {
                                              if (type == ui::Widget::TouchEventType::ENDED)
                                              {
                                                  const auto director = Director::getInstance();
                                                  if (director)
                                                  {
                                                      director->end();
                                                  }
                                              }
                                          });
        mainMenu->addChild(buttonExit);
    }

    const auto scoreLabel = Label::createWithSystemFont(StringUtils::format("Max score: %.0f", maxScore), "Arial", 24);
    if(scoreLabel)
    {
        scoreLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
        scoreLabel->setAnchorPoint(Vec2(0,0.5));
        scoreLabel->setPosition(Vec2(0,
                                     director->getVisibleSize().height - scoreLabel->getContentSize().height / 2.f));
        mainMenu->addChild(scoreLabel);
    }
}

void MainScene::setupGameplay()
{
    gameplayEnds = false;

    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    gameplayLayer = Layer::create();
    if(!gameplayLayer)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(gameplayLayer));
        return;
    }
    this->addChild(gameplayLayer);

    backgroundSprite = Sprite::create("Background.png");
    if (backgroundSprite)
    {
        gameplayLayer->addChild(backgroundSprite, 0);
        backgroundSprite->setAnchorPoint(Vec2(0, 0));
        bgMoving();
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(backgroundSprite));
    }

    playerShip = PlayerShip::create();
    if (playerShip)
    {
        gameplayLayer->addChild(playerShip, 1);

        playerShip->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 0.08f);
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(playerShip));
    }

    restartEnemyFactory();

    scoreLabel = Label::createWithSystemFont(StringUtils::format("Score: %.0f", playerShip->getScore()), "Arial", 24);
    if(scoreLabel)
    {
        scoreLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
        scoreLabel->setAnchorPoint(Vec2(0,0.5));
        scoreLabel->setPosition(Vec2(0,
                                     director->getVisibleSize().height - scoreLabel->getContentSize().height / 2.f));
        gameplayLayer->addChild(scoreLabel, 1);
    }

    hpLabel = Label::createWithSystemFont(StringUtils::format("Hp: %i", playerShip->getHp()), "Arial", 24);
    if(hpLabel)
    {
        hpLabel->setAlignment(cocos2d::TextHAlignment::LEFT);
        hpLabel->setAnchorPoint(Vec2(0,0.5));
        hpLabel->setPosition(Vec2(0,
                                     hpLabel->getContentSize().height / 2.f));
        gameplayLayer->addChild(hpLabel, 1);
    }
}

void MainScene::releaseMainMenu()
{
    mainMenu->removeFromParent();
    mainMenu = nullptr;
}

void MainScene::releaseGameplay()
{
    gameplayLayer->removeFromParent();
    gameplayLayer = nullptr;
    enemyFactory = nullptr;
    playerShip = nullptr;
    scoreLabel = nullptr;
    hpLabel = nullptr;
    backgroundSprite = nullptr;
}

void MainScene::gameplayEnd(float interval)
{
    this->schedule(CC_SCHEDULE_SELECTOR(MainScene::gameplayEndCallback), interval, 0, interval);

    enemyFactory->unscheduleUpdate();
}

void MainScene::restartEnemyFactory()
{
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    if(enemyFactory)
    {
        enemyFactory->removeFromParent();
        enemyFactory = nullptr;
    }

    enemyFactory = EnemyFactory::create();
    if(enemyFactory)
    {
        gameplayLayer->addChild(enemyFactory);

        enemyFactory->setPosition(
                director->getVisibleSize().width / 2.f,
                director->getVisibleSize().height * 1.5f);

        enemyFactory->runAction(MoveTo::create(3.f, Vec2(director->getVisibleSize().width / 2.f,
                                                         director->getVisibleSize().height * 0.8f)));
    }
    else
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(enemyFactory));
    }
}

void MainScene::gameplayEndCallback(float delay)
{
    gameplayEnds = true;
    const auto director = Director::getInstance();
    if(!director)
    {
        CCLOGERROR("%s", GENERATE_ERROR_MESSAGE(director));
        return;
    }

    const auto repeatButton = ui::Button::create(REPEAT_BUTTON_SPRITE,REPEAT_BUTTON_SPRITE,REPEAT_BUTTON_SPRITE);
    if(repeatButton)
    {
        repeatButton->setPosition(
                Vec2(director->getVisibleSize().width / 2.f, director->getVisibleSize().height / 2.f + 40.f));
        repeatButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type)
                                           {
                                               if (type == ui::Widget::TouchEventType::ENDED)
                                               {
                                                   releaseGameplay();
                                                   setupGameplay();
                                               }
                                           });
        gameplayLayer->addChild(repeatButton, 1);
    }

    const auto exitButton = ui::Button::create(EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE,EXIT_BUTTON_SPRITE);
    if(exitButton)
    {
        exitButton->setPosition(
                Vec2(director->getVisibleSize().width / 2.f, director->getVisibleSize().height / 2.f - 40.f));
        exitButton->addTouchEventListener([&](Ref *sender, ui::Widget::TouchEventType type)
                                          {
                                              if (type == ui::Widget::TouchEventType::ENDED)
                                              {
                                                  releaseGameplay();
                                                  setupMainMenu();
                                              }
                                          });
        gameplayLayer->addChild(exitButton, 1);
    }
}

void MainScene::bgMoving()
{
    if(!backgroundSprite) return;

    backgroundSprite->setPosition(0, 0);

    const auto moveDown = MoveTo::create(7.f, Vec2(0, -600));
    const auto callbackEnd = CallFunc::create(CC_CALLBACK_0(MainScene::bgMoving, this));

    const auto seq = Sequence::create(moveDown, callbackEnd, nullptr);

    backgroundSprite->runAction(seq);
}
