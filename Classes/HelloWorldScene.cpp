#include "HelloWorldScene.h"
#include <iomanip>
using namespace std;

Scene* HelloWorld::createScene()
{
    auto scene = cocos2d::Scene::create();
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
    this->colorLayer = new LayerColor;
    this->colorLayer->initWithColor(Color4B(255, 255, 255, 255));
    this->colorLayer->setScale(2.0f);
    this->addChild(this->colorLayer, 1);

    // create sprite
    this->sprite = Sprite::create("sprite.png");
    Size screenSize = Director::getInstance()->getWinSize();
    this->sprite->setPosition(Point(screenSize.width / 2.0f, screenSize.height / 2.0f));
    this->sprite->setAnchorPoint(Point(0.5f, 0.55f)); // nudge the anchor point upward because of the shadow
    this->addChild(this->sprite, 2);

    // listen for touch events
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    // set the appropriate resource directory for this device
    FileUtils::getInstance()->addSearchResolutionsOrder("HD");
    // load and cache the texture and sprite frames
    auto cacher = SpriteFrameCache::getInstance();
    cacher->addSpriteFramesWithFile("ninja.plist");

    Sprite* someSprite = new Sprite;
    someSprite->initWithSpriteFrameName("ninja-stopped0000.png");
    someSprite->setPosition(Vec2(200, 200));
    this->addChild(someSprite, 3);
    // get the sprite frame
    SpriteFrame* frame =
        cacher->getSpriteFrameByName("ninja-sidekick-e0007.png");

    // set someSprite's display frame
    someSprite->setSpriteFrame(frame);
    // load all the animation frames into an array
    const int kNumberOfFrames = 13;
    Vector<SpriteFrame*> frames;
    for (int i = 0; i < kNumberOfFrames; i++)
    {
        stringstream ss;
        ss << "ninja-sidekick-e" << setfill('0') << setw(4) << i << ".png";
        frames.pushBack(cacher->getSpriteFrameByName(ss.str()));
    }

    // play the animation
    Animation* anim = new Animation;
    anim->initWithSpriteFrames(frames, 0.1f);
    someSprite->runAction(RepeatForever::create(Animate::create(anim)));

    return true;
}

Point HelloWorld::touchToPoint(Touch* touch)
{
    // convert the touch object to a position in our cocos2d space
    return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
}

bool HelloWorld::isTouchingSprite(Touch* touch)
{
    // here's one way, but includes the rectangular white space around our sprite
    //return CGRectContainsPoint(this->sprite->boundingBox(), this->touchToPoint(touch));

    // this way is more intuitive for the user because it ignores the white space.
    // it works by calculating the distance between the sprite's center and the touch point,
    // and seeing if that distance is less than the sprite's radius
    return (this->sprite->getPosition().getDistance(this->touchToPoint(touch)) < 100.0f);
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    // reset touch offset
    this->touchOffset = Point::ZERO;

    if (touch && this->isTouchingSprite(touch)) {
        // calculate offset from sprite to touch point
        this->touchOffset = this->sprite->getPosition() - this->touchToPoint(touch);
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event) {
    // set the new sprite position
    if (touch && touchOffset.x && touchOffset.y)
        this->sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);
}

void HelloWorld::onTouchEnded(Touch* touch, Event* event) {
    if (touch && touchOffset.x && touchOffset.y)
    {
        // set the new sprite position
        this->sprite->setPosition(this->touchToPoint(touch) + this->touchOffset);

        // stop any existing actions and reset the scale
        this->sprite->stopAllActions();
        this->sprite->setScale(1.0f);

        // animate letting go of the sprite
        this->sprite->runAction(Sequence::create(
            ScaleBy::create(0.125f, 1.111f),
            ScaleBy::create(0.125f, 0.9f),
            nullptr
        ));
    }
}
