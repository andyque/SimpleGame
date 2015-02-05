#include "GameScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;


Scene* GameScene::createScene()
{
    //this line will add physics to our scene
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
//    layer->setPhysicsWorld(scene->getPhysicsWorld());

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
    
    // create a static PhysicsBody
    auto playerSize = _player->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(playerSize.width , playerSize.height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
    //cocos2d-x lack the collision group, I think we should add it
    
    //this line will let the physicsBody won't collide with anyother physics body with default collisionBitMask
    physicsBody->setCategoryBitmask(0);
    
    // sprite will use physicsBody
    _player->setPhysicsBody(physicsBody);
    
    //add contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    this->schedule(schedule_selector(GameScene::addMonster), 1.5);
    
    
    //3 enable touch
    //we can also call this->setTouchEnabled(true);
//    this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    //but it is deprecated in v3.x
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    //the second node parameter can't be nullptr
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);
    
    //play background musci
    SimpleAudioEngine::getInstance()->playBackgroundMusic("background-music-aac.caf",true);
    
    return true;
}

bool GameScene::onContactBegan(cocos2d::PhysicsContact &contact)
{
    auto bodyA = contact.getShapeA()->getBody()->getNode();
    auto bodyB = contact.getShapeB()->getBody()->getNode();
    
    bodyA->removeFromParent();
    bodyB->removeFromParent();
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
    
    //add physics
    auto projectileSize = projectile->getContentSize();
    auto physicsBody = PhysicsBody::createCircle(projectileSize.width/2 );
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    
    projectile->setPhysicsBody(physicsBody);
    
    //4
    auto actionMove = MoveTo::create(1.5f, targetPosition);
    auto actionRemove = RemoveSelf::create();
    projectile->runAction(Sequence::create(actionMove,actionRemove, nullptr));
    
    
    SimpleAudioEngine::getInstance()->playEffect("pew-pew-lei.caf");
    
    return true;
}


void GameScene::addMonster(float dt)
{
    //in c++11, we prefer auto in local variable decleration over regular class pointers
    auto monster = Sprite::create("monster.png");
    
    //add physics
    auto monsterSize = monster->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(monsterSize.width , monsterSize.height),
                                              PhysicsMaterial(0.1f, 1.0f, 0.0f));
    //cocos2d-x lack the collision group, I think we should add it
    physicsBody->setContactTestBitmask(0xFFFFFFFF);
    
    monster->setPhysicsBody(physicsBody);
 
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