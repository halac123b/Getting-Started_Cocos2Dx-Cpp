#include "GraphicScene.h"

using namespace cocos2d;


Scene* GraphicScene::createScene() {
	auto scene = Scene::create();
	auto layer = GraphicScene::create();
	scene->addChild(layer);
	return scene;
}

bool GraphicScene::init() {
	if (!Layer::init()) {
		return false;
	}
    auto sprite = Sprite::create("image.png");
    auto sprite2 = Sprite::create("image2.png");
    sprite->setAnchorPoint(Vec2(0.0, 0.0));
    sprite2->setAnchorPoint(Vec2(0.0, 0.0));

    sprite->addChild(sprite2);

    sprite->setPosition(100, 100);
    sprite2->setPosition(0, 0);

    this->addChild(sprite, 0);

    return true;
}
