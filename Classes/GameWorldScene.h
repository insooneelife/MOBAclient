#ifndef __GAMEWORLD_SCENE_H__
#define __GAMEWORLD_SCENE_H__

//------------------------------------------------------------------------
//
//  Name:   GameWorldScene.h
//
//  Desc:   This class makes the game play scene. 
//			So it has a layer for show game entity,
//			and a camera for user's view,
//			and gameworld for data interact.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <memory>
#include "cocos2d.h"
#include "GameWorld.h"

#include "ui\UIImageView.h"
#include "ui\UIButton.h"
#include "ui\UIText.h"
#include "ui\UILoadingBar.h"
#include "ui\UIWidget.h"
#include "ui\UITextField.h"
#include "ui\UIListView.h"

class GameWorld;
class ItemStoreLayer;
class OptionLayer;
class KillDeathLayer;

class GameWorldScene : public cocos2d::Layer 
{
public:
	enum {
		KEBOARD_Q, KEBOARD_W, KEBOARD_E, KEBOARD_R, KEBOARD_D,
		KEBOARD_F, KEBOARD_B, KEBOARD_A
	};

	//static cocos2d::DrawNode* debug_layer;

	// implement the "static create()" method manually
	CREATE_FUNC(GameWorldScene);
	virtual bool init() override;

	static cocos2d::Scene* scene();

	~GameWorldScene();

	cocos2d::Node* getGamePanel() const;

	cocos2d::Node* getUiPanel() const;

	//character
	cocos2d::Node* getCharacterPanel() const;

	cocos2d::ui::ImageView* getCharacterImage() const;

	cocos2d::ui::Text* getCharacterLevel() const;

	cocos2d::ui::LoadingBar* getCharacterExp() const;

	cocos2d::ui::Button* getItem(int indx) const;

	cocos2d::ui::Button* getStore() const;

	cocos2d::ui::Text* getDamage() const;

	cocos2d::ui::Text* getDefensive() const;

	cocos2d::ui::Text* getSpeed() const;

	//skill
	cocos2d::Node* getSkillPanel() const;

	cocos2d::ui::Button* getSkill(int indx) const;

	cocos2d::ui::LoadingBar* getHpBar() const;

	cocos2d::ui::LoadingBar* getMpBar() const;

	cocos2d::ui::Text* getHpText() const;

	cocos2d::ui::Text* getMpText() const;

	//minimap
	cocos2d::Node* getMiniMapPanel() const;

	cocos2d::ui::ImageView* getMiniMapImage() const;

	//team
	cocos2d::Node* getTeamStatePanel() const;

	cocos2d::ui::ImageView* getTeamUser(int indx) const;

	cocos2d::ui::LoadingBar* getTeamHp(int indx) const;

	void setLookAt(cocos2d::Vec2 _look_at);


	// a selector callback
	void menuCloseCallback(cocos2d::Ref* sender);

	void mouseMoveUpdate(float fDelta);

	bool isOnUI(float x, float y);

	void setGameChatMessage(const std::string& name, const std::string& chat_message);

	void updateCharacterInformation(
		int level,
		int damage,
		int defence,
		int move_speed,
		int attack_speed,
		int magic_damage,
		int magic_defence,
		int hp,
		int mp);

	void onOffLayer(bool& active, cocos2d::Layer* layer);

	void updateUserSkillImage(const std::string& cname);

	void updateUserCharacterImage(const std::string& cname);

	void updateItemStoreLayer();

	virtual void onKeyPressed(
		cocos2d::EventKeyboard::KeyCode key_code,
		cocos2d::Event* e) override;

	virtual void onKeyReleased(
		cocos2d::EventKeyboard::KeyCode key_code,
		cocos2d::Event* e) override;

	//schedule processes of gameworld or other events
	virtual void update(float fDelta) override;

private:
	// scene interacts with game world.
	std::unique_ptr<GameWorld>	_game_world;

	// camera which we look at.
	cocos2d::Camera*			_view_camera;
	cocos2d::Vec3				_look_at;

	// for mouse movement
	cocos2d::Vec2				_move_pos;
	float						_prev_mouse_x;
	float						_prev_mouse_y;

	// for time checking
	float						_render_time;
	float						_update_time;


	// all visible entity renders on this layer.
	cocos2d::Layer*				_view_layer;

	cocos2d::Sprite*			_current_clicked_image;

	cocos2d::Node*				_game_panel;

	cocos2d::Node*				_ui_panel;

	//character
	cocos2d::Node*				_character_panel;

	cocos2d::ui::ImageView*		_character_image;

	cocos2d::ui::Text*			_character_level;

	cocos2d::ui::LoadingBar*	_character_exp;

	cocos2d::ui::Button*		_item[8];	//item 1~7 use

	cocos2d::ui::Text*			_damage;

	cocos2d::ui::Text*			_defensive;

	cocos2d::ui::Text*			_speed;

	cocos2d::ui::Text* _attack_speed;
	cocos2d::ui::Text* _magic_damage;
	cocos2d::ui::Text* _magic_defensive;

	//store

	ItemStoreLayer*				_item_store_layer;

	cocos2d::ui::Button*		_store_button;

	bool						_store_active;


	//option
	OptionLayer*				_option_layer;
	bool						_option_active;

	//kill death
	KillDeathLayer*				_kill_death_layer;
	bool						_kill_death_active;


	//skill
	cocos2d::Node*				_skill_panel;

	cocos2d::ui::Button*		_skill[8];

	cocos2d::ui::LoadingBar*	_hp_bar;

	cocos2d::ui::LoadingBar*	_mp_bar;

	cocos2d::ui::Text*			_hp_text;

	cocos2d::ui::Text*			_mp_text;

	//map
	cocos2d::Node*				_mini_map_panel;

	cocos2d::ui::ImageView*		_mini_map_image;

	//team
	cocos2d::Node*				_team_state_panel;

	cocos2d::ui::ImageView*		_team_user[5];//1~4 use

	cocos2d::ui::LoadingBar*	_team_hp[5];//1~4 use

	// information
	cocos2d::Node* _clicked_character_panel;
	cocos2d::ui::ImageView* _clicked_character_image;

	cocos2d::ui::Text* _clicked_character_level;
	cocos2d::ui::LoadingBar* _clicked_character_exp;
	cocos2d::ui::Text* _clicked_character_damage;
	cocos2d::ui::Text* _clicked_character_defensive;
	cocos2d::ui::Text* _clicked_character_speed;
	cocos2d::ui::Text* _clicked_character_attack_speed;
	cocos2d::ui::Text* _clicked_character_magic_damage;
	cocos2d::ui::Text* _clicked_character_magic_defensive;

	cocos2d::ui::LoadingBar* _clicked_character_hp_bar;
	cocos2d::ui::LoadingBar* _clicked_character_mp_bar;
	cocos2d::ui::Text* _clicked_character_hp_text;
	cocos2d::ui::Text* _clicked_character_mp_text;


	//chat
	cocos2d::Node* _game_chat_panel;
	cocos2d::ui::ListView* _game_chat_list_view;
	cocos2d::ui::TextField* _game_chat_text_field;
	cocos2d::TextFieldTTF*		_write_text_field;
	bool						_activate_text_field;

};

#endif
