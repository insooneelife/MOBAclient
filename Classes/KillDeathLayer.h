#ifndef __KILLDEATH_LAYER_H__
#define __KILLDEATH_LAYER_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
#include "ui\UIImageView.h"
#include "ui\UITextField.h"
#include "ui\UIScrollView.h"
#include "ui\UIText.h"
#include "ui\UICheckBox.h"

class KillDeathLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Layer* createLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(KillDeathLayer);


private:

	cocos2d::Node* _kill_death_panel;

	cocos2d::Node* _blue_team_death_panel;

	cocos2d::Node* _blue_team_kill_death_panel[3];
	cocos2d::ui::ImageView* _blue_team_kill_death_champ_img[3];
	cocos2d::ui::ImageView* _blue_team_kill_death_item[3][6];
	cocos2d::ui::Text* _blue_team_kill_death_lv_text[3];
	cocos2d::ui::Text* _blue_team_kill_death_kill_text[3];
	cocos2d::ui::Text* _blue_team_kill_death_death_text[3];
	cocos2d::ui::Text* _blue_team_kill_death_cs_text[3];


	cocos2d::Node* _red_team_death_panel;

	cocos2d::Node* _red_team_kill_death_panel[3];
	cocos2d::ui::ImageView* _red_team_kill_death_champ_img[3];
	cocos2d::ui::ImageView* _red_team_kill_death_item[3][6];
	cocos2d::ui::Text* _red_team_kill_death_lv_text[3];
	cocos2d::ui::Text* _red_team_kill_death_kill_text[3];
	cocos2d::ui::Text* _red_team_kill_death_death_text[3];
	cocos2d::ui::Text* _red_team_kill_death_cs_text[3];


};

#endif
