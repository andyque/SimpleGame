#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void addMonster(float dt);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    bool onContactBegan(cocos2d::PhysicsContact &contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    Sprite *_player;
    int _monstersDestroyed;
};

#endif // __HELLOWORLD_SCENE_H__
