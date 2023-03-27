#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorld : public Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;

    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);

    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingSprite(Touch* touch);

    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

    CREATE_FUNC(HelloWorld);

private:
    Sprite* sprite;
    LayerColor* colorLayer;
    Point touchOffset;
};