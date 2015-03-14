//
//  GameOverScene.cpp
//  SimpleGame
//
//  Created by guanghui on 3/14/15.
//
//

#include "GameOverScene.h"
#include "HelloWorldScene.h"



bool GameOverScene::init(cocos2d::Size size, bool won)
{
    bool ret = true;
    do {
        if (!Scene::init()) {
            ret = false;
            break;
        }
        auto origin = Director::getInstance()->getVisibleOrigin();
        auto winSize = Director::getInstance()->getVisibleSize();

        //do your own init here
        
        //1. clear scene with white background
        auto background = DrawNode::create();
        background->drawSolidRect(origin, winSize, Color4F::WHITE);
        this->addChild(background);
        
        //2
        auto message = won ? "You Won!" : "You Lost :[";
     
        
        //3
        auto label = Label::createWithSystemFont(message, "Chalkduster", 40);
        label->setTextColor(Color4B::BLACK);
        label->setPosition(Vec2(winSize.width/2, winSize.height/2));
        addChild(label);
        
        //4
        runAction(Sequence::create(DelayTime::create(3.0), CallFunc::create([](){
            auto reveal = TransitionFlipX::create(0.5, HelloWorld::createScene());
            Director::getInstance()->replaceScene(reveal);
        }),nullptr));
        
        
    } while (0);
    
    return ret;
}

GameOverScene* GameOverScene::create(cocos2d::Size size, bool won)
{
    auto scene = new GameOverScene;
    if (scene && scene->init(size, won)) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}