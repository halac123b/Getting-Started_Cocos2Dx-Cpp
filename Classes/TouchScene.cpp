#include "TouchScene.h"

USING_NS_CC;

Scene* TouchScene::createScene()
{
    auto scene = Scene::create();
    auto layer = TouchScene::create();
    scene->addChild(layer);

    return scene;
}

bool TouchScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
   auto sprite = Sprite::create("HelloWorld.png");
   //sprite->setAnchorPoint(Vec2(0.0, 0.0));
   //sprite->setPosition(0, 0);
   sprite->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2));

    // Add a "touch" event listener to our sprite
   auto touchListener = EventListenerTouchOneByOne::create();
   touchListener->onTouchBegan = [](Touch* touch, Event* event) -> bool {

      auto bounds = event->getCurrentTarget()->getBoundingBox();
      //Vec2 position = event->getCurrentTarget()->getParent()->convertToWorldSpace(event->getCurrentTarget()->getPosition());
      //bounds.origin += position;
      Vec2 position = event->getCurrentTarget()->convertToWorldSpace(event->getCurrentTarget()->getPosition());
      cocos2d::log("%f %f", bounds.origin.x, bounds.origin.y);
      cocos2d::log("%f %f", position.x, position.y);
      cocos2d::log("%f %f", touch->getLocation().x, touch->getLocation().y);
      Vec2 position2 = touch->getLocation();
      position2.y -= 160;
      if (bounds.containsPoint(position2)){
         std::stringstream touchDetails;
         touchDetails << "Touched at OpenGL coordinates: " << 
            touch->getLocation().x << "," << touch->getLocation().y << std::endl <<
            "Touched at UI coordinate: " << 
            touch->getLocationInView().x << "," << touch->getLocationInView().y << std::endl <<
            "Touched at local coordinate:" <<
            event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).x << "," <<  
            event->getCurrentTarget()->convertToNodeSpace(touch->getLocation()).y << std::endl <<
            "Touch moved by:" << touch->getDelta().x << "," << touch->getDelta().y;

            MessageBox(touchDetails.str().c_str(), "Touched");
         }
      return true;
      };

   Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener,sprite);
   this->addChild(sprite, 0);
    
    return true;
}