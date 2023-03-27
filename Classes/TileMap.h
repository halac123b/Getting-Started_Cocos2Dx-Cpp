#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class TileMap : public Layer
{
public:
    static Scene* createScene();
    virtual bool init() override;
    void setViewPointCenter(CCPoint position);

    void setPlayerPosition(Point position);

    bool onTouchBegan(Touch* touch, Event* event);

    void onTouchEnded(Touch* touch, Event* event);

    Point tileCoordForPosition(Point position);
    CREATE_FUNC(TileMap);

private:
    TMXTiledMap* _tileMap;
    TMXLayer* _background;
    Sprite* _player;
    TMXLayer* _meta;
};