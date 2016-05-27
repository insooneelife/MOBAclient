#include "AppDelegate.h"

#include <vector>
#include <string>

#include "SimpleAudioEngine.h"

#include "GameWorldScene.h"
#include "AppMacros.h"
#include "ParameterLoader.h"
#include "LoginScene.h"
#include "TeamSelectScene.h"
#include "EndingScene.h"

USING_NS_CC;
using namespace std;

AppDelegate::AppDelegate() 
{}

AppDelegate::~AppDelegate() 
{}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() 
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        //glview = GLViewImpl::createWithFullScreen("GAME");
		glview = GLViewImpl::create("GAME");
    }

    director->setOpenGLView(glview);

	//cocos2d::Size(2048, 1536)
	//cocos2d::Size(1024, 768)
	//cocos2d::Size(480, 320)

    // Set the design resolution
    glview->setDesignResolutionSize(
		1920,
		1080,
		ResolutionPolicy::SHOW_ALL);

    
    vector<string> searchPath;

    // set searching path
    FileUtils::getInstance()->setSearchPaths(searchPath);
	
    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60.0f);

    // create a scene. it's an autorelease object

	director->pushScene(GameWorldScene::scene());

	director->pushScene(TeamSelectScene::scene());
	
	auto scene = LoginScene::scene();
	
	FileUtils::getInstance()->addSearchPath("backgroundSound");
	auto background_music = CocosDenshion::SimpleAudioEngine::getInstance();
	background_music->playBackgroundMusic("loginSceneSound.mp3", false);

	//auto scene = TeamSelectScene::scene();
	//auto scene = GameWorldScene::scene();
	//auto scene = EndingScene::createScene();


    // run
    director->runWithScene(scene);


    return true;
}

// This function will be called when the app is INACTIVE. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is ACTIVE again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
