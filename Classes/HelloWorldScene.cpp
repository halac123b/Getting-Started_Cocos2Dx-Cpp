/*
#include "HelloWorldScene.h"

using namespace cocos2d;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
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

    auto label = Label::createWithSystemFont("Hello World", "Arial", 96);
    label->setAnchorPoint(cocos2d::Vec2(0.0, 0.0));
    this->addChild(label, 1);

    return true;
}
*/

/*
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

    sprite = Sprite::create("Veyron.png");
    sprite->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
    this->addChild(sprite, 0);

    this->scheduleUpdate();
    return true;
}

void HelloWorld::update(float delta) {
    auto position = sprite->getPosition();
    position.x -= 250 * delta;
    if (position.x < 0 - (sprite->getBoundingBox().size.width / 2))
        position.x = this->getBoundingBox().getMaxX() + sprite->getBoundingBox().size.width / 2;
    sprite->setPosition(position);
}
*/

/*
#include "HelloWorldScene.h"

cocos2d::Scene* HelloWorld::createScene()
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

    sprite = cocos2d::Sprite::create("Veyron.png");
    sprite->setPosition(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY());
    this->addChild(sprite, 0);

    auto listener = cocos2d::EventListenerKeyboard::create();
    /*
    listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)->void {
        // This is where our different actions are going to be implemented
        auto action = cocos2d::MoveTo::create(2, cocos2d::Vec2(0, 0));
        sprite->runAction(action);
    };
    */
    /*
    listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)->void {
        cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
        actions.pushBack(cocos2d::ScaleBy::create(1.5, 1.5));
        actions.pushBack(cocos2d::TintTo::create(1.5, 255, 0, 0));
        actions.pushBack(cocos2d::FadeTo::create(1.5, 30));
        auto sequence = cocos2d::Sequence::create(actions);

        sprite->runAction(sequence);
    };
    */
    /*
    listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)->void {
        cocos2d::Vector<cocos2d::FiniteTimeAction*> actions;
        actions.pushBack(cocos2d::MoveTo::create(1, cocos2d::Vec2(0, 0)));
        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::Place::create(cocos2d::Vec2(
            this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY())));
        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::FlipX::create(true));
        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::FlipY::create(true));
        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::Hide::create());
        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::Show::create());
        actions.pushBack(cocos2d::DelayTime::create(1));

        actions.pushBack(cocos2d::CallFunc::create([=]()->void {
            this->setColor(cocos2d::Color3B::RED);
            }));

        actions.pushBack(cocos2d::DelayTime::create(1));
        actions.pushBack(cocos2d::RemoveSelf::create(false));

        auto sequence = cocos2d::Sequence::create(actions);
        sprite->runAction(sequence);
    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}
*/

#include "HelloWorldScene.h"

using namespace cocos2d;

cocos2d::Scene* HelloWorld::createScene()
{
    auto scene = cocos2d::Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if (!LayerColor::initWithColor(cocos2d::Color4B::BLACK))
    {
        return false;
    }

    label = cocos2d::Label::createWithSystemFont("Press space to pause all, 1 to pause left", "Arial", 30);
    label->setPosition(cocos2d::Vec2(this->getBoundingBox().getMidX(), this->getBoundingBox().getMaxY() - 20));

    sprite = cocos2d::Sprite::create("Veyron.png");
    sprite2 = cocos2d::Sprite::create("Veyron.png");
    sprite->setPosition(250, this->getBoundingBox().getMidY());
    sprite2->setPosition(700, this->getBoundingBox().getMidY());

    auto rotate = cocos2d::RotateBy::create(1, 45);
    auto rotate2 = cocos2d::RotateBy::create(1, -45);

    auto repeat1 = cocos2d::RepeatForever::create(rotate);
    auto repeat2 = cocos2d::RepeatForever::create(rotate2);

    this->addChild(label, 0);
    this->addChild(sprite, 0);
    this->addChild(sprite2, 0);

    sprite->runAction(repeat1);
    sprite2->runAction(repeat2);
    auto listener = cocos2d::EventListenerKeyboard::create();
    listener->onKeyPressed = [=](cocos2d::EventKeyboard::KeyCode code, cocos2d::Event* event)->void {
        // On Spacebar, Pause/Unpause all actions and updates
        if (code == cocos2d::EventKeyboard::KeyCode::KEY_SPACE) {
            if (pausedNodes.size()) {
                cocos2d::Director::getInstance()->getActionManager()->resumeTargets(pausedNodes);
                pausedNodes.clear();
                //spritePaused = false; // In case user currently has 1 pressed too
            }
            else
                pausedNodes = cocos2d::Director::getInstance()->getActionManager()->pauseAllRunningActions();
            label->setString("Spacebar pressed");
        }
        // Pause/UnPause just sprite 1
        if (code == cocos2d::EventKeyboard::KeyCode::KEY_1) {
            if (spritePaused)
                sprite->resumeSchedulerAndActions();
            else
                sprite->pauseSchedulerAndActions();
            spritePaused = !spritePaused;
            label->setString("1 pressed");
        }

    };

    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}