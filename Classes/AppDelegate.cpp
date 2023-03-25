#include "AppDelegate.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
        glview = GLViewImpl::create("HelloWorld");
        glview->setFrameSize(960, 640);
        director->setOpenGLView(glview);
    }

    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
}

void AppDelegate::applicationWillEnterForeground()
{
}