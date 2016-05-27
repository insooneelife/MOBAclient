#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include "GameWorldScene.h"
#include "GameWorld.h"
#include "Entity\Character.h"
#include "GameMap.h"
#include "AppMacros.h"
#include "EntityManager.h"
#include "Util.h"
#include "Timer\PrecisionTimer.h"
#include "Timer\CrudeTimer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "DebugRenderer.h"

#include "ItemStoreLayer.h"
#include "OptionLayer.h"
#include "KillDeathLayer.h"

#include "Network\Packet\PacketFactory.h"
#include "Network\GameClient\GameClientManager.h"

#include "Database.h"

USING_NS_CC;
using namespace cocostudio::timeline;

PrecisionTimer timer;

Scene* GameWorldScene::scene() 
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	
	// 'layer' is an autorelease object
	GameWorldScene* layer = GameWorldScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameWorldScene::init()
{
	if (!Layer::init())
		return false;

	PacketFactory::getInstance().setGameWorldScene(this);

	FileUtils::getInstance()->addSearchPath(Prm.CharacterFolderName);
	FileUtils::getInstance()->addSearchPath(Prm.StructureFolderName);
	FileUtils::getInstance()->addSearchPath(Prm.FontFolderName);
	FileUtils::getInstance()->addSearchPath(Prm.ParticleFolderName);
	FileUtils::getInstance()->addSearchPath("animations");
	FileUtils::getInstance()->addSearchPath("effectSound");
	FileUtils::getInstance()->addSearchPath("UI");


	//--------------------------GameWorld Setting-------------------------------//
	_view_layer = Layer::create();
	this->addChild(_view_layer);

	Debug.initCustomShapePanel(false, 1);
	_view_layer->addChild(Debug.getCustomShapePanel());

	Debug.initBoundingShapePanel(false, 1);
	_view_layer->addChild(Debug.getBoundingShapePanel());

	Debug.initBoundingBoxPanel(false, 1);
	_view_layer->addChild(Debug.getBoundingBoxPanel());

	Debug.initViewCirclePanel(false, 1);
	_view_layer->addChild(Debug.getViewCirclePanel());

	Debug.initGraphPanel(false, 1);
	_view_layer->addChild(Debug.getGraphPanel());

	Debug.initPathLinePanel(false, 1);
	_view_layer->addChild(Debug.getPathLinePanel());

	Debug.initCellBoxPanel(false, 1);
	_view_layer->addChild(Debug.getCellBoxPanel());

	cocos2d::DrawNode* circle = DrawNode::create();
	circle->drawSolidCircle(Vec2(0, 0), 5, (float)(2 * M_PI), 40, Color4F::YELLOW);
	circle->drawCircle(Vec2(4880, 2440), 5, (float)(2 * M_PI), 40, false, Color4F::BLUE);
	circle->drawLine(Vec2(-514, 0), Vec2(0, 0), Color4F::BLUE);
	_view_layer->addChild(circle);

	timer.smoothUpdatesOn();
	//start the timer
	timer.start();

	// if you want to use shared_ptr without constructing, use like this.
	_game_world.reset(new GameWorld(_view_layer, _look_at.x, _look_at.y));


	//this->schedule(CC_SCHEDULE_SELECTOR(GameWorldScene::update), Prm.GameSpeedInterval);
	this->scheduleUpdate();

	//------------------------User Interface Setting-----------------------------//

	//_item_store_layer = dynamic_cast<ItemStoreLayer*>(ItemStoreLayer::createLayer());
	//this->addChild(_item_store_layer);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	auto closeItem = MenuItemImage::create(
		"UI/CloseNormal.png",
		"UI/CloseSelected.png",
		CC_CALLBACK_1(GameWorldScene::menuCloseCallback, this));

	closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	auto label = Label::createWithTTF(
		"Game World",
		Prm.FontFolderName + "/" + Prm.FontName +".ttf",
		TITLE_FONT_SIZE);
	label->setColor(Color3B::RED);
	// position the label on the center of the screen
	label->setPosition(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height - label->getContentSize().height);

	// add the label as a child to this layer
	this->addChild(label, 1);

	this->schedule(CC_SCHEDULE_SELECTOR(GameWorldScene::mouseMoveUpdate), 0.02f);
	

	_current_clicked_image = nullptr;

	auto rootNode = CSLoader::createNode("UI/MainScene.csb");
	addChild(rootNode);
	rootNode->setTag(0);

	_game_panel = rootNode->getChildByTag(1);

	//ui
	_ui_panel = rootNode->getChildByTag(2);

	auto ui_image = dynamic_cast<ui::ImageView*>(_ui_panel->getChildByTag(294));

	ui_image->loadTexture("UI/UI(1500x750).png");

	//character
	_character_panel = _ui_panel->getChildByTag(3);
	_character_image = dynamic_cast<ui::ImageView*>(_character_panel->getChildByTag(4));
	_character_level = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(5));
	_character_exp = dynamic_cast<ui::LoadingBar*>(_character_panel->getChildByTag(6));
	_attack_speed = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(403));
	_magic_damage = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(404));
	_magic_defensive = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(405));
	_damage = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(15));
	_defensive = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(16));
	_speed = dynamic_cast<ui::Text*>(_character_panel->getChildByTag(17));

	for (int i = 1; i < 8; i++) 
	{
		_item[i] = dynamic_cast<ui::Button*>(_character_panel->getChildByTag(6 + i));
		_item[i]->setZoomScale(0.4f);
		_item[i]->setPressedActionEnabled(true);
		//_item[i]->setBright(false);
		_item[i]->setScale9Enabled(true);
		_item[i]->setCapInsets(Rect(3, 3, 5, 5));
		_item[i]->addClickEventListener([=](Ref*) {
			//_item[i]->setBright(true);
			CCLOG("!!!!!!!!!click item %d \n", i);
		});
	}

	//skill
	_skill_panel = _ui_panel->getChildByTag(18);

	for (int i = 0; i < 8; i++) {
		_skill[i] = dynamic_cast<ui::Button*>(_skill_panel->getChildByTag(19 + i));
		_skill[i]->setZoomScale(0.4f);
		_skill[i]->setPressedActionEnabled(true);
		_skill[i]->setScale9Enabled(true);
		_skill[i]->setCapInsets(Rect(3, 3, 5, 5));
		_skill[i]->setSwallowTouches(true);
		_skill[i]->addClickEventListener([=](Ref*) 
		{
			//_skill[i]->setBright(true);
			CCLOG("!!!!!!!!!click skill%d \n", i);
		});
	}



	_hp_bar = dynamic_cast<ui::LoadingBar*>(_skill_panel->getChildByTag(27));
	_hp_bar->setOpacity(55);
	_mp_bar = dynamic_cast<ui::LoadingBar*>(_skill_panel->getChildByTag(28));
	_mp_bar->setOpacity(55);
	_hp_text = dynamic_cast<ui::Text*>(_skill_panel->getChildByTag(29));
	_mp_text = dynamic_cast<ui::Text*>(_skill_panel->getChildByTag(30));

	
	//mini map
	_mini_map_panel = _ui_panel->getChildByTag(31);
	_mini_map_image = dynamic_cast<ui::ImageView*>(_mini_map_panel->getChildByTag(32));
	_mini_map_image->loadTexture("minimap.png");

	//team
	_team_state_panel = _ui_panel->getChildByTag(33);

	for (int i = 1; i < 5; i++) {
		_team_user[i] = dynamic_cast<ui::ImageView*>(_team_state_panel->getChildByTag(33 + i));
	}
	for (int i = 1; i < 5; i++) {
		_team_hp[i] = dynamic_cast<ui::LoadingBar*>(_team_state_panel->getChildByTag(37 + i));
	}


	//chat
	_game_chat_panel = _ui_panel->getChildByTag(400);
	_game_chat_list_view = dynamic_cast<ui::ListView*>(_game_chat_panel->getChildByTag(401));
	_game_chat_text_field = dynamic_cast<ui::TextField*>(_game_chat_panel->getChildByTag(402));

	_game_chat_list_view->setScrollBarPositionFromCornerForVertical(Vec2(10, 7));
	_game_chat_list_view->setScrollBarColor(Color3B::WHITE);
	_game_chat_list_view->setScrollBarAutoHideEnabled(false);
	_game_chat_list_view->setScrollBarWidth(15);
	_game_chat_list_view->setScrollBarOpacity(40);
	_game_chat_list_view->setBounceEnabled(true);
	_game_chat_list_view->setClippingEnabled(true);
	_game_chat_list_view->removeAllChildrenWithCleanup(true);


	_write_text_field =
		CCTextFieldTTF::textFieldWithPlaceHolder(
			"messages", _game_chat_text_field->getContentSize(), TextHAlignment::LEFT, "Arial", 20);
	_write_text_field->setPosition(_game_chat_text_field->getPosition());
	_write_text_field->setAnchorPoint(_game_chat_text_field->getAnchorPoint());
	_game_chat_text_field->getParent()->addChild(_write_text_field);

	_game_chat_text_field->setVisible(false);
	_game_chat_text_field->setTouchEnabled(false);

	_write_text_field->detachWithIME();
	_activate_text_field = false;

	// information
	_clicked_character_panel = _ui_panel->getChildByTag(422);

	auto char_info_img = dynamic_cast<ui::ImageView*>(_clicked_character_panel->getChildByTag(440));
	char_info_img->loadTexture("UI/enemy.png");

	_clicked_character_image = dynamic_cast<ui::ImageView*>(_clicked_character_panel->getChildByTag(423));
	_clicked_character_level = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(424));
	_clicked_character_exp = dynamic_cast<ui::LoadingBar*>(_clicked_character_panel->getChildByTag(425));
	_clicked_character_damage = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(427));
	_clicked_character_defensive = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(428));
	_clicked_character_speed = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(429));
	_clicked_character_attack_speed = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(430));
	_clicked_character_magic_damage = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(431));
	_clicked_character_magic_defensive = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(432));

	_clicked_character_hp_bar = dynamic_cast<ui::LoadingBar*>(_clicked_character_panel->getChildByTag(441));
	_clicked_character_mp_bar = dynamic_cast<ui::LoadingBar*>(_clicked_character_panel->getChildByTag(442));
	_clicked_character_hp_text = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(444));
	_clicked_character_mp_text = dynamic_cast<ui::Text*>(_clicked_character_panel->getChildByTag(445));


	// item store
	_item_store_layer = dynamic_cast<ItemStoreLayer*>(ItemStoreLayer::createLayer());
	_item_store_layer->setVisible(false);
	_item_store_layer->setTouchEnabled(false);
	_ui_panel->addChild(_item_store_layer);
	_store_active = false;

	//store button
	_store_button = dynamic_cast<ui::Button*>(_character_panel->getChildByTag(14));
	_store_button->setZoomScale(0.4f);
	_store_button->setPressedActionEnabled(true);
	//_store_button->setBright(false);
	_store_button->setScale9Enabled(true);
	_store_button->setCapInsets(Rect(3, 3, 5, 5));
	_store_button->addClickEventListener([=](Ref*) {
		onOffLayer(_store_active, _item_store_layer);
	});


	//option 
	_option_layer = dynamic_cast<OptionLayer*>(OptionLayer::createLayer());
	_option_layer->setVisible(false);
	_option_layer->setTouchEnabled(false);
	_ui_panel->addChild(_option_layer);
	_option_active = false;


	//kill death
	_kill_death_layer = dynamic_cast<KillDeathLayer*>(KillDeathLayer::createLayer());
	_kill_death_layer->setVisible(false);
	_kill_death_layer->setTouchEnabled(false);
	_ui_panel->addChild(_kill_death_layer);
	_kill_death_active = false;
	



	//---------------------------- Event Setting -------------------------------//

	auto listener = cocos2d::EventListenerMouse::create();
	listener->onMouseDown = [this](cocos2d::EventMouse* e)
	{
		if (isOnUI(e->getCursorX(), e->getCursorY()))
		{
			return;
		}

		CCLOG("mouse : %f %f   frame : %f %f", e->getCursorX(), e->getCursorY(), Prm.FrameSizeX, Prm.FrameSizeY);

		//1028 is world's size of Frame width
		const float ratio = 1028 / Prm.FrameSizeX * Prm.CameraDistance / 500;

		if (e->getMouseButton() == MOUSE_BUTTON_LEFT)
		{
			cocos2d::Vec2 cpos = 
				util::convertMousePosToWorldPos(
				Vec2(e->getCursorX(), e->getCursorY()),
				Vec2(_look_at.x, _look_at.y),
				ratio);

			_game_world->setCurrentClickedEntityID(0);
			// check if some character is clicked
			for (auto iter = std::begin(_game_world->getCharacters());
				iter != std::end(_game_world->getCharacters()); iter++)
			{
				auto bounding_box = iter->second->getNowSprite()->getBoundingBox();
				float min_x = bounding_box.getMinX();
				float min_y = bounding_box.getMinY();
				float max_x = bounding_box.getMaxX();
				float max_y = bounding_box.getMaxY();

				if ((min_x <= cpos.x && cpos.x <= max_x) && (min_y <= cpos.y && cpos.y <= max_y))
				{
					_game_world->setCurrentClickedEntityID(iter->first);
					iter->second->getNowSprite()->setColor(cocos2d::Color3B::GRAY);

					_clicked_character_image->loadTexture(
						iter->second->getName() + "/" + iter->second->getName() + "_main.png");

					break;
				}
			}

			_game_world->clickLeftMouseButton(cpos);
		}
		else if (e->getMouseButton() == MOUSE_BUTTON_RIGHT)
		{
			_game_world->clickRightMouseButton(
				util::convertMousePosToWorldPos(
					Vec2(e->getCursorX(), e->getCursorY()),
					Vec2(_look_at.x, _look_at.y),
					ratio));
		}
		else if (e->getMouseButton() == MOUSE_BUTTON_MIDDLE) {}
		else {}
	};

	listener->onMouseMove = [this](cocos2d::EventMouse* e)
	{
		float mx = e->getCursorX();
		float my = e->getCursorY();
		_move_pos = Vec2(0, 0);

		_move_pos = util::borderDirection(mx, my, Prm.BorderSize);
		_move_pos *= Prm.MouseMoveVelocity;

		//1028 is world's size of Frame width
		const float ratio = 1028 / Prm.FrameSizeX * Prm.CameraDistance / 500;

		_game_world->moveMouse(
			util::convertMousePosToWorldPos(
			Vec2(e->getCursorX(), e->getCursorY()),
			Vec2(_look_at.x, _look_at.y), 
				ratio));

		if (e->getMouseButton() == MOUSE_BUTTON_LEFT) {}
		else if (e->getMouseButton() == MOUSE_BUTTON_RIGHT) {}
		else if (e->getMouseButton() == MOUSE_BUTTON_MIDDLE) {}
		else {}
	};

	listener->onMouseUp = [this](cocos2d::EventMouse* e)
	{
		if (e->getMouseButton() == MOUSE_BUTTON_LEFT) 
		{
			_game_world->releaseLeftMouseButton();
		}
		else if (e->getMouseButton() == MOUSE_BUTTON_RIGHT) 
		{}
		else if (e->getMouseButton() == MOUSE_BUTTON_MIDDLE) {}
		else {}
	};

	//마우스 이벤트 등록
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _view_layer);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _game_panel);

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(GameWorldScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(GameWorldScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);


	//--------------------------Camera Setting-------------------------------//

	//this is the layer, when adding camera to it, 
	//all its children will be affect only when you set the second parameter to true
	_view_layer->setCameraMask((unsigned short)CameraFlag::USER2, true);

	_view_camera = Camera::createPerspective(
		60, (float)visibleSize.width / visibleSize.height, 1.0, 1000 + Prm.CameraDistance);
	_view_camera->setCameraFlag(CameraFlag::USER2);

	//Vec2 agent_pos = (_game_world->getCharacters()[0])->getPos();
	//_look_at = Vec3(agent_pos.x, agent_pos.y, 0.0);

	//for see where agent is
	//the calling order matters, we should first call setPosition3D, then call lookAt.

	_look_at = Vec3(Prm.StartViewX, Prm.StartViewY, 0.0);

	_view_camera->setPosition3D(_look_at + Vec3(0.0, 0.0, Prm.CameraDistance));
	_view_camera->lookAt(_look_at + Vec3(0.0, 1.0, 0.0));
	_view_layer->addChild(_view_camera);

	return true;
}

GameWorldScene::~GameWorldScene() 
{
	CCLOG("~GameWorldScene()");
}

void GameWorldScene::update(float fDelta) 
{
	static bool once = true;
	if (once)
	{
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		CCLOG("FrameSize : %f %f", glview->getFrameSize().width, glview->getFrameSize().height);
		//glview->setFrameSize(Prm.FrameSizeX, Prm.FrameSizeY);
		once = false;
	}

	int begin = clock();

	Debug.getBoundingShapePanel()->clear();
	Debug.getBoundingBoxPanel()->clear();
	Debug.getViewCirclePanel()->clear();
	Debug.getPathLinePanel()->clear();

	_game_world->render();
	_game_world->update(timer.timeElapsed());

	auto iter = _game_world->getCharacters().find(_game_world->getCurrentClickedEntityID());
	if (iter != std::end(_game_world->getCharacters()))
	{
		updateCharacterInformation(
			0,
			iter->second->getDamage(),
			0,
			iter->second->getMoveSpeed(),
			50,
			iter->second->getMagicDamage(),
			0,
			iter->second->getHealth(),
			iter->second->getMana());
	}
	else
	{
		updateCharacterInformation(0, 0, 0, 0, 0, 0, 0, 0, 0);
		_clicked_character_image->loadTexture("Button_Disable.png");
	}
}

void GameWorldScene::mouseMoveUpdate(float fDelta) {
	float mx = _move_pos.x;
	float my = _move_pos.y;

	if (mx > 0)
		_look_at.x = std::min(_look_at.x + mx, (float)(_game_world->getGameMap()->getSizeX()));
	else
		_look_at.x = std::max(_look_at.x + mx, 0.0f);

	if (my > 0)
		_look_at.y = std::min(_look_at.y + my, (float)(_game_world->getGameMap()->getSizeY()));
	else
		_look_at.y = std::max(_look_at.y + my, 0.0f);
		
	_view_camera->setPosition3D(_look_at + Vec3(0.0, 0.0, Prm.CameraDistance));
	
	_view_camera->lookAt(_look_at + Vec3(0.0, 1.0, 0.0));
}

bool GameWorldScene::isOnUI(float x, float y)
{
	if (geometry::pointInRectangle(
		Vec2(x, y),
		Vec2(_skill_panel->getBoundingBox().getMaxX(),
			_skill_panel->getBoundingBox().getMaxY()),
		Vec2(_skill_panel->getBoundingBox().getMinX(), 
			_skill_panel->getBoundingBox().getMinY()))) 
	{
		return true;
	}
	else if (geometry::pointInRectangle(
		Vec2(x, y),
		Vec2(_character_panel->getBoundingBox().getMaxX(),
			_character_panel->getBoundingBox().getMaxY()),
		Vec2(_character_panel->getBoundingBox().getMinX(),
			_character_panel->getBoundingBox().getMinY()))) 
	{
		return true;
	}
	else if (geometry::pointInRectangle(
		Vec2(x, y),
		Vec2(_mini_map_panel->getBoundingBox().getMaxX(),
			_mini_map_panel->getBoundingBox().getMaxY()),
		Vec2(_mini_map_panel->getBoundingBox().getMinX(),
			_mini_map_panel->getBoundingBox().getMinY()))) 
	{
		return true;
	}
	else if (geometry::pointInRectangle(
		Vec2(x, y),
		Vec2(_team_state_panel->getBoundingBox().getMaxX(),
			_team_state_panel->getBoundingBox().getMaxY()),
		Vec2(_team_state_panel->getBoundingBox().getMinX(),
			_team_state_panel->getBoundingBox().getMinY()))) 
	{
		return true;
	}
	return false;
}

void GameWorldScene::setGameChatMessage(const std::string& name, const std::string& chat_message)
{
	static int msg_cnt = 0;
	auto newText = ui::Text::create();
	newText->setFontSize(20);
	newText->setString(name + " : " + chat_message);

	_game_chat_list_view->pushBackCustomItem(newText);
	_game_chat_list_view->scrollToItem(msg_cnt, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);

	if (msg_cnt >= MAX_MESSAGES)
	{
		_game_chat_list_view->removeItem(0);
	}
	else
	{
		msg_cnt++;
	}
}

void GameWorldScene::updateCharacterInformation(
	int level,
	int damage,
	int defence,
	int move_speed,
	int attack_speed,
	int magic_damage,
	int magic_defence,
	int hp,
	int mp)
{
	std::stringstream ss;
	ss << level;
	_clicked_character_level->setString(ss.str());
	ss.str("");

	ss << damage;
	_clicked_character_damage->setString(ss.str());
	ss.str("");

	ss << defence;
	_clicked_character_defensive->setString(ss.str());
	ss.str("");

	ss << move_speed;
	_clicked_character_speed->setString(ss.str());
	ss.str("");

	ss << attack_speed;
	_clicked_character_attack_speed->setString(ss.str());
	ss.str("");

	ss << magic_damage;
	_clicked_character_magic_damage->setString(ss.str());
	ss.str("");

	ss << magic_defence;
	_clicked_character_magic_defensive->setString(ss.str());
	ss.str("");

	ss << hp;
	_clicked_character_hp_text->setString(ss.str());
	ss.str("");

	ss << mp;
	_clicked_character_mp_text->setString(ss.str());
	ss.str("");
}

void GameWorldScene::menuCloseCallback(Ref* sender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void GameWorldScene::onOffLayer(bool& active, cocos2d::Layer* layer)
{
	active = !active;
	layer->setTouchEnabled(active);
	layer->setVisible(active);
}

void GameWorldScene::updateUserSkillImage(const std::string& cname)
{
	auto iter = DB.getInstance().getCharacterSkill()[cname];
	
	std::array<std::string, 4> skill_names = 
	{
		std::get<0>(iter) , std::get<1>(iter) , std::get<2>(iter) , std::get<3>(iter)
	};

	for (size_t i = 0; i < skill_names.size(); i++)
	{
		_skill[i]->loadTextureNormal("skills/images/" + skill_names[i] + ".png");
		_skill[i]->loadTexturePressed("skills/images/" + skill_names[i] + ".png");
	}
}

void GameWorldScene::updateUserCharacterImage(const std::string& cname)
{
	std::string filename = "characters/images/" + cname + ".png";
	_character_image->loadTexture(filename);
}

void GameWorldScene::updateItemStoreLayer()
{
	_item_store_layer->updateItemImage();
}

void GameWorldScene::onKeyPressed(EventKeyboard::KeyCode key_code, Event* event)
{
	//if enter key pressed, then user can write messages
	//if again they can write messages.
	if (key_code == EventKeyboard::KeyCode::KEY_ENTER)
	{
		_activate_text_field = !_activate_text_field;
		std::string name = GameClientManager::getInstance().getGameClient()->getClientName();

		if (_activate_text_field)
		{
			_write_text_field->attachWithIME();
			_write_text_field->setString("");
			_write_text_field->setPlaceHolder(name + " : ");
		}
		else
		{
			GameClientManager::getInstance().send(
				PacketFactory::getInstance().createPacketGameChatMessageToServer(
					GameClientManager::getInstance().getGameClient()->getId(),
					_write_text_field->getString()));

			_write_text_field->setString("");
			_write_text_field->detachWithIME();
		}
	}

	if (_activate_text_field)
		return;


	int index = 0;
	switch (key_code)
	{
	case EventKeyboard::KeyCode::KEY_Q:
		index = 0;
		_game_world->setCurrentSkillKey(GameWorld::SKILL_Q);
		CCLOG("!!!!!!!!!press Q %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_W:
		index = 1;
		_game_world->setCurrentSkillKey(GameWorld::SKILL_W);
		CCLOG("!!!!!!!!!press W %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_E:
		index = 2;
		_game_world->setCurrentSkillKey(GameWorld::SKILL_E);
		CCLOG("!!!!!!!!!press E %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_R:
		index = 3;
		_game_world->setCurrentSkillKey(GameWorld::SKILL_R);
		CCLOG("!!!!!!!!!press R %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_A:
		index = 4;
		_game_world->setCurrentSkillKey(GameWorld::ATTACK_A);
		CCLOG("!!!!!!!!!press A %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_B:
		index = 5;
		CCLOG("!!!!!!!!!press B %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_D:
		index = 6;
		CCLOG("!!!!!!!!!press D %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_F:
		index = 7;
		CCLOG("!!!!!!!!!press F %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_1:
		index = 8;
		CCLOG("!!!!!!!!!press 1 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_2:
		index = 9;
		CCLOG("!!!!!!!!!press 2 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_3:
		index = 10;
		CCLOG("!!!!!!!!!press 3 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_4:
		index = 11;
		CCLOG("!!!!!!!!!press 4 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_5:
		index = 12;
		CCLOG("!!!!!!!!!press 5 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_6:
		index = 13;
		CCLOG("!!!!!!!!!press 6 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_7:
		index = 14;
		CCLOG("!!!!!!!!!press 7 %d \n", index);
		break;

	case EventKeyboard::KeyCode::KEY_P:
		index = 14;
		onOffLayer(_store_active, _item_store_layer);
		break;

	case EventKeyboard::KeyCode::KEY_ESCAPE:
		onOffLayer(_option_active, _option_layer);
		break;

	case EventKeyboard::KeyCode::KEY_TAB:
		onOffLayer(_kill_death_active, _kill_death_layer);
		break;
	}
}

void GameWorldScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (_activate_text_field)
		return;
}


cocos2d::Node* GameWorldScene::getGamePanel() const 
{
	return _game_panel;
}

cocos2d::Node* GameWorldScene::getUiPanel() const 
{
	return _ui_panel;
}

//char
cocos2d::Node* GameWorldScene::getCharacterPanel() const 
{
	return _character_panel;
}

cocos2d::ui::ImageView* GameWorldScene::getCharacterImage() const 
{
	return _character_image;
}

cocos2d::ui::Text* GameWorldScene::getCharacterLevel() const 
{
	return _character_level;
}

cocos2d::ui::LoadingBar* GameWorldScene::getCharacterExp() const 
{
	return _character_exp;
}

cocos2d::ui::Button* GameWorldScene::getItem(int indx) const 
{
	return _item[indx];
}

cocos2d::ui::Button* GameWorldScene::getStore() const 
{
	return _store_button;
}

cocos2d::ui::Text* GameWorldScene::getDamage() const 
{
	return _damage;
}

cocos2d::ui::Text* GameWorldScene::getDefensive() const 
{
	return _defensive;
}

cocos2d::ui::Text* GameWorldScene::getSpeed() const 
{
	return _speed;
}


//skill
cocos2d::Node* GameWorldScene::getSkillPanel() const 
{
	return _skill_panel;
}

cocos2d::ui::Button* GameWorldScene::getSkill(int indx) const 
{
	return _skill[indx];
}

cocos2d::ui::LoadingBar* GameWorldScene::getHpBar() const 
{
	return _hp_bar;
}

cocos2d::ui::LoadingBar* GameWorldScene::getMpBar() const 
{
	return _mp_bar;
}

cocos2d::ui::Text* GameWorldScene::getHpText() const 
{
	return _hp_text;
}

cocos2d::ui::Text* GameWorldScene::getMpText() const 
{
	return _mp_text;
}

//map
cocos2d::Node* GameWorldScene::getMiniMapPanel() const 
{
	return _mini_map_panel;
}

cocos2d::ui::ImageView* GameWorldScene::getMiniMapImage() const 
{
	return _mini_map_image;
}

//team
cocos2d::Node* GameWorldScene::getTeamStatePanel() const 
{
	return _team_state_panel;
}

cocos2d::ui::ImageView* GameWorldScene::getTeamUser(int indx) const 
{
	return _team_user[indx];
}

cocos2d::ui::LoadingBar* GameWorldScene::getTeamHp(int indx) const 
{
	return _team_hp[indx];
}

void GameWorldScene::setLookAt(cocos2d::Vec2 look_at)
{
	_look_at = Vec3(look_at.x, look_at.y, _look_at.z);
}