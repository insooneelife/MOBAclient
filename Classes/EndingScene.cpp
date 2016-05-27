#include "EndingScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "GlobalValues.h"
#include "Network\Packet\PacketFactory.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* EndingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = EndingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool EndingScene::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
	PacketFactory::getInstance().setEndingScene(this);


	FileUtils::getInstance()->addSearchPath("UI");

    auto rootNode = CSLoader::createNode("EndingScene.csb");
	addChild(rootNode);
	rootNode->setTag(0);

	_ending_panel = rootNode->getChildByTag(1787);
	
	_red_win = dynamic_cast<ui::Text*>(_ending_panel->getChildByTag(47));
	_red_win->setVisible(false);
	_blue_win = dynamic_cast<ui::Text*>(_ending_panel->getChildByTag(48));
	_blue_win->setVisible(false);

    return true;
}


void EndingScene::win(const std::string& name, int team)
{
	FileUtils::getInstance()->addSearchPath("backgroundSound");
	auto background_music = CocosDenshion::SimpleAudioEngine::getInstance();
	background_music->playBackgroundMusic("endingSceneSound.mp3", false);
	if (team == TEAM_RED)
	{
		//music cue!
		_red_win->setVisible(true);
	}
	else
	{
		_blue_win->setVisible(true);
	}
}