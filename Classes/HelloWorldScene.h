#pragma once

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void addMonster(float dt);
    bool onTouchBegan(Touch* touch, Event* unused_event);
    bool onContactBegan(PhysicsContact& contact);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
    Sprite* _player;
};
