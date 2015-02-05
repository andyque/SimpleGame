#include "GameScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto winSize = Director::getInstance()->getVisibleSize();
    
    auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, Color4F(0.6,0.6,0.6,1.0));
    this->addChild(background);
    
    srand(time(NULL));
    
    //add player
    _player = Sprite::create("player.png");
    _player->setPosition(Vec2(winSize.width/8, winSize.height/2));
    this->addChild(_player);
    
    
    this->schedule(schedule_selector(GameScene::addMonster), 1.5);
    
    
    //3 enable touch
    //we can also call this->setTouchEnabled(true);
//    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    //but it is deprecated in v3.x
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    //the second node parameter can't be nullptr
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);
    
    
    return true;
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    //this line is only for debug purpose
    CCLOG("clicked");
    //here the node is _player which is passed in in addEventListenerWithSceneGraphPriority
//    auto node = unused_event->getCurrentTarget();
    //1
    Vec2 touchLocation = touch->getLocation();
    
    //2
    Vec2 offset = touchLocation - _player->getPosition();
    float ratio = offset.y / offset.x;
    int targetX = _player->getContentSize().width/2 + this->getContentSize().width;
    int targetY = (targetX * ratio) + _player->getPosition().y;
    Vec2 targetPosition = Vec2(targetX, targetY);
    
    //3
    auto projectile = Sprite::create("projectile.png");
    projectile->setPosition(_player->getPosition());
    this->addChild(projectile);
    
    //4
    auto actionMove = MoveTo::create(1.5f, targetPosition);
    auto actionRemove = RemoveSelf::create();
    projectile->runAction(Sequence::create(actionMove,actionRemove, nullptr));
    
    return true;
}


void GameScene::addMonster(float dt)
{
    //in c++11, we prefer auto in local variable decleration over regular class pointers
    auto monster = Sprite::create("monster.png");
    
    //1
    auto monsterContentSize = monster->getContentSize();
    auto selfContentSize = this->getContentSize();
    int minY = monsterContentSize.height/2;
    int maxY = selfContentSize.height - monsterContentSize.height/2;
    int rangeY = maxY - minY;
    //we must call srand(time(NULL)) before calling rand() to get a random number
    int randomY = (rand() % rangeY) + minY;
    
    //2
    monster->setPosition(Vec2(selfContentSize.width + monsterContentSize.width/2, randomY));
    this->addChild(monster);
    
    //3
    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    int randomDuration = (rand() % rangeDuration) + minDuration;
    
    //4
    auto actionMove = MoveTo::create(randomDuration, Vec2(-monsterContentSize.width/2, randomY));
    auto actionRemove = RemoveSelf::create();
    //in c++ 11, it's better to use nullptr instead of NULL
    monster->runAction(Sequence::create(actionMove,actionRemove, nullptr));
}