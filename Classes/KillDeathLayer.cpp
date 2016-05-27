#include "KillDeathLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
USING_NS_CC;

using namespace cocostudio::timeline;

Layer* KillDeathLayer::createLayer()
{
    // 'layer' is an autorelease object
    auto layer = KillDeathLayer::create();

    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool KillDeathLayer::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
	FileUtils::getInstance()->addSearchPath("UI");
    auto rootNode = CSLoader::createNode("KillDeathLayer.csb");
	addChild(rootNode);
	rootNode->setTag(0);
	rootNode->setPosition(cocos2d::Vec2(400, 200));

	_kill_death_panel = rootNode->getChildByTag(567);

	_blue_team_death_panel=_kill_death_panel->getChildByTag(568);

	
	int idx = 572;
	for (int i = 0; i < 3; i++) 
	{
		_blue_team_kill_death_panel[i] = _blue_team_death_panel->getChildByTag(idx++);
		_blue_team_kill_death_champ_img[i] = dynamic_cast<ui::ImageView*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));

		for (int j = 0; j < 6; j++) {
			_blue_team_kill_death_item[i][j] = dynamic_cast<ui::ImageView*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));
		}
		
		_blue_team_kill_death_lv_text[i] = dynamic_cast<ui::Text*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));
		_blue_team_kill_death_kill_text[i] = dynamic_cast<ui::Text*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));
		_blue_team_kill_death_death_text[i] = dynamic_cast<ui::Text*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));
		_blue_team_kill_death_cs_text[i] = dynamic_cast<ui::Text*>(_blue_team_kill_death_panel[i]->getChildByTag(idx++));
	}


	

	_red_team_death_panel = _kill_death_panel->getChildByTag(608);

	idx = 612;
	for (int i = 0; i < 3; i++) {

		_red_team_kill_death_panel[i] = _red_team_death_panel->getChildByTag(idx++);
		_red_team_kill_death_champ_img[i] = dynamic_cast<ui::ImageView*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));

		for (int j = 0; j < 6; j++) {
			_red_team_kill_death_item[i][j] = dynamic_cast<ui::ImageView*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));
		}

		_red_team_kill_death_lv_text[i] = dynamic_cast<ui::Text*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));
		_red_team_kill_death_kill_text[i] = dynamic_cast<ui::Text*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));
		_red_team_kill_death_death_text[i] = dynamic_cast<ui::Text*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));
		_red_team_kill_death_cs_text[i] = dynamic_cast<ui::Text*>(_red_team_kill_death_panel[i]->getChildByTag(idx++));
	}
		

    return true;
}
