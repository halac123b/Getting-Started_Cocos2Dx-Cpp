#include "TileMap.h"

Scene* TileMap::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = TileMap::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool TileMap::init()
{
    if (!Layer::init())
    {
        return false;
    }

    _tileMap = new TMXTiledMap();
    _tileMap->initWithTMXFile("TileMap.tmx");
    _background = _tileMap->layerNamed("Background");
    _meta = _tileMap->layerNamed("Meta");
    _meta->setVisible(false);

    this->addChild(_tileMap);

    TMXObjectGroup* objectGroup = _tileMap->objectGroupNamed("Objects");

    if (objectGroup == NULL) {
        log("tile map has no objects object layer");
        return false;
    }

    ValueMap spawnPoint = objectGroup->objectNamed("SpawnPoint");

    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();

    _player = new Sprite();
    _player->initWithFile("Player.png");
    _player->setPosition(Vec2(x, y));

    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(TileMap::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(TileMap::onTouchEnded, this);
    Director::sharedDirector()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void TileMap::setViewPointCenter(CCPoint position) {

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height / 2);
    CCPoint actualPosition = ccp(x, y);

    CCPoint centerOfView = ccp(winSize.width / 2, winSize.height / 2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

bool TileMap::onTouchBegan(CCTouch* touch, CCEvent* event)
{
    return true;
}

void TileMap::setPlayerPosition(CCPoint position) {
    Point tileCoord = this->tileCoordForPosition(position);
    int tileGid = _meta->tileGIDAt(tileCoord);
    if (tileGid) {      
        return;
    }
    _player->setPosition(position);
}

void TileMap::onTouchEnded(CCTouch* touch, CCEvent* event)
{
    Point touchLocation = touch->getLocationInView();
    touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
    touchLocation = this->convertToNodeSpace(touchLocation);

    Point playerPos = _player->getPosition();
    Point diff = ccpSub(touchLocation, playerPos);

    if (abs(diff.x) > abs(diff.y)) {
        if (diff.x > 0) {
            playerPos.x += _tileMap->getTileSize().width;
        }
        else {
            playerPos.x -= _tileMap->getTileSize().width;
        }
    }
    else {
        if (diff.y > 0) {
            playerPos.y += _tileMap->getTileSize().height;
        }
        else {
            playerPos.y -= _tileMap->getTileSize().height;
        }
    }

    // safety check on the bounds of the map
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0)
    {
        this->setPlayerPosition(playerPos);
    }

    this->setViewPointCenter(_player->getPosition());
}


Point TileMap::tileCoordForPosition(Point position)
{
    int x = (int)(position.x / _tileMap->getTileSize().width);
    int y = (int)(((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height);
    return Point(x, y);
}
