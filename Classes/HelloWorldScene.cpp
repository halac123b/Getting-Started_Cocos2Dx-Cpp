#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

#define BACKGROUND_MUSIC_SFX  "background-music-aac.mp3"
#define PEW_PEW_SFX           "pew-pew-lei.mp3"

enum class PhysicsCategory {
    None = 0,
    Monster = (1 << 0),    // 1
    Projectile = (1 << 1), // 2
    All = (int)PhysicsCategory::Monster | (int)PhysicsCategory::Projectile // 3
};

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setGravity(Vec2(0, 0));
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if (!Layer::init()) {
        return false;
    }
    // Lấy kích thước và tọa độ của màn hình
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto winSize = Director::getInstance()->getVisibleSize();
    // Vẽ nền màn hình màu xám
    auto background = DrawNode::create();
    background->drawSolidRect(origin, winSize, Color4F(0.6, 0.6, 0.6, 1.0));
    this->addChild(background);
    // Thêm sprite player vào
    _player = Sprite::create("player.png");
    _player->setPosition(Vec2(winSize.width * 0.1, winSize.height * 0.5));
    this->addChild(_player);

    // Spawn enemy
    srand((unsigned int)time(nullptr));
    this->schedule(schedule_selector(HelloWorld::addMonster), 1.5);

    auto eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, _player);

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegan, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

    SimpleAudioEngine::getInstance()->playBackgroundMusic(BACKGROUND_MUSIC_SFX, true);

    return true;
}

void HelloWorld::addMonster(float dt) {
    auto monster = Sprite::create("monster.png");

    // 1
    auto monsterSize = monster->getContentSize();
    auto physicsBody = PhysicsBody::createBox(Size(monsterSize.width, monsterSize.height),
        PhysicsMaterial(0.1f, 1.0f, 0.0f));
    // 2
    physicsBody->setDynamic(true);
    // 3
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Monster);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Projectile);

    monster->setPhysicsBody(physicsBody);

    // Set random position for monster to spawn
    auto monsterContentSize = monster->getContentSize();
    auto selfContentSize = this->getContentSize();
    int minY = monsterContentSize.height / 2;
    int maxY = selfContentSize.height - monsterContentSize.height / 2;
    int rangeY = maxY - minY;
    int randomY = (rand() % rangeY) + minY;

    monster->setPosition(Vec2(selfContentSize.width + monsterContentSize.width / 2, randomY));
    this->addChild(monster);

    // Set random velocity
    int minDuration = 2.0;
    int maxDuration = 4.0;
    int rangeDuration = maxDuration - minDuration;
    int randomDuration = (rand() % rangeDuration) + minDuration;

    // Action of monster
    auto actionMove = MoveTo::create(randomDuration, Vec2(-monsterContentSize.width / 2, randomY));
    auto actionRemove = RemoveSelf::create();
    monster->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* unused_event) {
    // 1  - Just an example for how to get the  _player object
    //auto node = unused_event->getCurrentTarget();

    // 2
    Vec2 touchLocation = touch->getLocation();
    Vec2 offset = touchLocation - _player->getPosition();

    // 3
    if (offset.x < 0) {
        return true;
    }

    // 4
    auto projectile = Sprite::create("projectile.png");

    auto projectileSize = projectile->getContentSize();
    auto physicsBody = PhysicsBody::createCircle(projectileSize.width / 2);
    physicsBody->setDynamic(true);
    physicsBody->setCategoryBitmask((int)PhysicsCategory::Projectile);
    physicsBody->setCollisionBitmask((int)PhysicsCategory::None);
    physicsBody->setContactTestBitmask((int)PhysicsCategory::Monster);
    projectile->setPhysicsBody(physicsBody);

    projectile->setPosition(_player->getPosition());
    this->addChild(projectile);

    // 5
    offset.normalize();
    auto shootAmount = offset * 1000;

    // 6
    auto realDest = shootAmount + projectile->getPosition();

    // 7
    auto actionMove = MoveTo::create(2.0f, realDest);
    auto actionRemove = RemoveSelf::create();
    projectile->runAction(Sequence::create(actionMove, actionRemove, nullptr));
    SimpleAudioEngine::getInstance()->playEffect(PEW_PEW_SFX);
    return true;
}

bool HelloWorld::onContactBegan(PhysicsContact& contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    nodeA->removeFromParent();
    nodeB->removeFromParent();
    return true;
}
