#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Constants.h"
#include "GameOverScene.h"

USING_NS_CC;
using namespace CocosDenshion;

enum class PhysicsCategory
{
    None = 0,
    Monster = (1 << 0),  //1
    Projectile = (1 << 1), //2
    All =   PhysicsCategory::Monster | PhysicsCategory::Projectile //3
};

Scene* HelloWorld::createScene()
{
    //this line will add physics to our scene
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0,0));
    //toggle debug draw
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
    

    //add player
    _player = Sprite::create("player.png");
    _player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5 ));
    this->addChild(_player);
    
    
    //spawn monster repeatly
    srand((unsigned int)time(nullptr));
    this->schedule(schedule_selector(HelloWorld::addMonster), 1.5);
    
    //3 enable touch
    //we can also call this->setTouchEnabled(true);
    //this->setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
    //but it is deprecated in v3.x
    
    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    //the second node parameter can't be nullptr
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);
    
    
    //add contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    //play background music
    SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC_SFX,true);
    
    _monstersDestroyed = 0;
    
    return true;
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    SimpleAudioEngine::getInstance()->playEffect(PEW_PEW_SFX);

    // 0  - get the  _player object
    //auto node = unused_event->getCurrentTarget();
    
    // 1 - Choose one of the touches to work with
    Vec2 touchLocation = touch->getLocation();
    
    // 2 - Set up initial location of projectile
    auto projectile = Sprite::create("projectile.png");
    projectile->setPosition(_player->getPosition());
    
    //add physics
    auto projectileSize = projectile->getContentSize();
    auto physicsBody = PhysicsBody::createCircle(projectileSize.width/2 );
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Monster);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    
    
    projectile->setPhysicsBody(physicsBody);
    
    // 3 - Determine offset of location to projectile
    Vec2 offset = touchLocation - _player->getPosition();
    
    // 4 - Bail out if you are shooting down or backwards
    if (offset.x < 0)
    {
        return true;
    }
    
    // 5 - OK to add now - you've double checked position
    this->addChild(projectile);


    // 6 - Get the direction of where to shoot
    offset.normalize();

    // 7 - Make it shoot far enough to be guaranteed off screen
    auto shootAmount = offset * 1000;

    // 8 - Add the shoot amount to the current position
    auto realDest = shootAmount + projectile->getPosition();

    // 9 - Create the actions
    auto actionMove = MoveTo::create(2.0f, realDest);
    auto actionRemove = RemoveSelf::create();
    
    projectile->runAction(Sequence::create(actionMove,actionRemove, nullptr));
    
    return true;
}

void HelloWorld::addMonster(float dt)
{
    //in c++11, we prefer auto in local variable decleration over regular class pointers
    auto monster = Sprite::create("monster.png");
    
    //add physics
    auto monsterSize = monster->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(monsterSize.width , monsterSize.height),
                                              PhysicsMaterial(0.1f, 1.0f, 0.0f));
    physicsBody->setDynamic(true);
    //cocos2d-x lack the collision group, I think we should add it
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    
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
   // monster->runAction(Sequence::create(actionMove,actionRemove, nullptr));
    

    auto loseAction = CallFunc::create([=](){
        auto reveal = TransitionFlipX::create(0.5, GameOverScene::create(this->getContentSize(), false));
        Director::getInstance()->replaceScene(reveal);
    });
    monster->runAction(Sequence::create(actionMove,loseAction, actionRemove,nullptr));
}

bool HelloWorld::onContactBegan(cocos2d::PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    
    //caculate the score
    _monstersDestroyed++;
    if (_monstersDestroyed > 3) {
        auto reveal = TransitionFlipX::create(0.5, GameOverScene::create(this->getContentSize(), true));
        Director::getInstance()->replaceScene(reveal);
    }
    
    nodeA->removeFromParent();
    nodeB->removeFromParent();
    return true;
}


