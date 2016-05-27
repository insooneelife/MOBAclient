#pragma once
#ifndef __TEAMSELECTSCENE_H__
#define __TEAMSELECTSCENE_H__

#include <map>
#include "cocos2d.h"
#include "ui\UIImageView.h"
#include "ui\UIButton.h"
#include "ui\UIText.h"
#include "ui\UILoadingBar.h"
#include "ui\UIWidget.h"
#include "ui\UIScrollView.h"
#include "ui\UIListView.h"
#include "ui\UITextField.h"


class TeamSelectScene : public cocos2d::Layer
{
public:

	enum { KEBOARD_Q, KEBOARD_W, KEBOARD_E, KEBOARD_R, KEBOARD_D, KEBOARD_F, KEBOARD_B, KEBOARD_A };

	enum { SLOT_SIZE = 6 };

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* scene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init() override;
	
	CREATE_FUNC(TeamSelectScene);

	virtual ~TeamSelectScene();

	cocos2d::ui::Text* getUserIdText(int slot);

	cocos2d::ui::ImageView* getUserCheckRed(int slot);

	cocos2d::ui::ImageView* getUserCheckBlue(int slot);

	cocos2d::ui::ImageView* getUserChampImage(int slot);

	cocos2d::ui::ImageView* getSelectUserCheckChamp(int slot);


	void setSlotNumber(int slot_number);

	int getSlotNumber() const;

	void clickedReadyFlag();

	int getReadyFlag() const;

	void setUserEventAlive(int num);

	void setRoomChatMessage(const std::string& name, const std::string& chat_message);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) override;

	void readyButtonEvent(
		cocos2d::Ref *pSender, 
		cocos2d::ui::Widget::TouchEventType type);

	void teamButtonEvent(
		cocos2d::Ref *pSender, 
		cocos2d::ui::Widget::TouchEventType type,
		int team);

	void championButtonEvent(
		cocos2d::Ref *pSender,
		cocos2d::ui::Widget::TouchEventType type, 
		const std::string& cname);

	virtual void update(float delta) override;

private:
	
	int _slot_number;
	int _ready_flag;

	cocos2d::Node* _team_panel;
	cocos2d::ui::ImageView* _team_panel_image;

	/////////////////////// team select //////////////////

	cocos2d::Node* _team_select_panel;

	//user1
	cocos2d::Node* _select_user_panel[6];
	cocos2d::ui::ImageView* _select_user_bg[6];
	cocos2d::ui::ImageView* _select_user_not_champ[6];
	cocos2d::ui::ImageView* _select_user_id_image[6];
	cocos2d::ui::Button* _select_user_team_red[6];
	cocos2d::ui::Button* _select_user_team_blue[6];
	cocos2d::ui::ImageView* _select_user_champ_image[6];
	cocos2d::ui::Text* _user_id_text[6];
	cocos2d::ui::ImageView* _select_user_check_champ[6];
	cocos2d::ui::ImageView* _select_user_check_red[6];
	cocos2d::ui::ImageView* _select_user_check_blue[6];

	cocos2d::ui::Button* _ready_button;
	cocos2d::ui::Button* _quit_button;


	//////////////////// champ && chat select ///////////////////////

	cocos2d::Node* _champ_and_chat_panel;
	cocos2d::ui::ImageView* _champ_bg_image;
	
	cocos2d::ui::ScrollView* _champ_scroll;
	cocos2d::ui::ImageView* _champ_scroll_image[2];
	cocos2d::Node* _champ_in_scroll_panel1;
	cocos2d::ui::Button*_champion_one[18];	
	
	cocos2d::Node* _champ_in_scroll_panel2;
	cocos2d::ui::Button*_champion_two[18];

	cocos2d::ui::ImageView* _chat_bind_image;
	cocos2d::ui::ImageView* _chat_bind_list_image;
	cocos2d::ui::ImageView* _chat_text_image;

	cocos2d::ui::ListView* _chat_listview;
	cocos2d::ui::Text* _chat_list[5];
	cocos2d::TextFieldTTF* _chat_text_field;

	bool					_activate_text_field;

	cocos2d::Node* _game_panel;
};

#endif
