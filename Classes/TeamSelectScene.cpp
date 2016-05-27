#include "TeamSelectScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "GlobalValues.h"
#include "Network/GameClient/GameClientManager.h"
#include "Network/GameClient/GameClient.h"
#include "Network/Packet/PacketFactory.h"
#include "ParameterLoader.h"
#include "Database.h"
#include "Util.h"

USING_NS_CC;

using namespace cocostudio::timeline;

TeamSelectScene::~TeamSelectScene()
{
	PacketFactory::getInstance().setTeamSelectScene(nullptr);
}

Scene* TeamSelectScene::scene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = TeamSelectScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool TeamSelectScene::init()
{
	
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	PacketFactory::getInstance().setTeamSelectScene(this);

	FileUtils::getInstance()->addSearchPath("UI");
	auto rootNode = CSLoader::createNode("TeamSelectScene.csb");
	addChild(rootNode);
	rootNode->setTag(0);
	//allPrint(rootNode);


	_slot_number = SLOT_SIZE;
	_ready_flag = CLICKED_NOT_READY;


	_team_panel= rootNode->getChildByTag(1000);
	_team_panel_image = dynamic_cast<ui::ImageView*>(_team_panel->getChildByTag(2001));


	/////////////////////// team select //////////////////

   _team_select_panel = _team_panel ->getChildByTag(1001);

      
	/////////////////////////user////////////////////////////////
   for (int i = 0; i <5; i++) {
	   _select_user_panel[i] = _team_select_panel->getChildByTag(i*11+1002);
	   _select_user_bg[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1003));
	   _select_user_not_champ[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1004));
	   _select_user_id_image[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1005));


	   _select_user_team_red[i] = dynamic_cast<ui::Button*>(_select_user_panel[i]->getChildByTag(i * 11 + 1006));

	   _select_user_team_blue[i] = dynamic_cast<ui::Button*>(_select_user_panel[i]->getChildByTag(i * 11 + 1007));

	   _select_user_champ_image[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1008));
	   _user_id_text[i] = dynamic_cast<ui::Text*>(_select_user_panel[i]->getChildByTag(i * 11 + 1009));
	   _select_user_check_champ[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1010));
	   _select_user_check_champ[i]->setVisible(false);
	   _select_user_check_red[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1011));
	   _select_user_check_red[i]->setVisible(false);
	   _select_user_check_blue[i] = dynamic_cast<ui::ImageView*>(_select_user_panel[i]->getChildByTag(i * 11 + 1012));
	   _select_user_check_blue[i]->setVisible(false);
   }

   _select_user_panel[5] = _team_select_panel->getChildByTag(2057);
   _select_user_bg[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2058));
   _select_user_not_champ[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2059));
   _select_user_id_image[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2060));


   _select_user_team_red[5] = dynamic_cast<ui::Button*>(_select_user_panel[5]->getChildByTag(2061));

   _select_user_team_blue[5] = dynamic_cast<ui::Button*>(_select_user_panel[5]->getChildByTag(2062));

   _select_user_champ_image[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2063));
   _user_id_text[5] = dynamic_cast<ui::Text*>(_select_user_panel[5]->getChildByTag(2064));
   _select_user_check_champ[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2065));
   _select_user_check_champ[5]->setVisible(false);
   _select_user_check_red[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2066));
   _select_user_check_red[5]->setVisible(false);
   _select_user_check_blue[5] = dynamic_cast<ui::ImageView*>(_select_user_panel[5]->getChildByTag(2067));
   _select_user_check_blue[5]->setVisible(false);


	_ready_button = dynamic_cast<ui::Button*>(_team_select_panel->getChildByTag(1058));
	_ready_button->setZoomScale(0.4f);
	_ready_button->setPressedActionEnabled(true);
	//_select_user1_team_red[0]->setBright(false);
	_ready_button->setScale9Enabled(true);
	_ready_button->setCapInsets(Rect(3, 3, 5, 5));
	_ready_button->addTouchEventListener(CC_CALLBACK_2(TeamSelectScene::readyButtonEvent, this));

	
	_quit_button = dynamic_cast<ui::Button*>(_team_select_panel->getChildByTag(1059));
	_quit_button->setZoomScale(0.4f);
	_quit_button->setPressedActionEnabled(true);
	//_select_user1_team_red[0]->setBright(false);
	_quit_button->setScale9Enabled(true);
	_quit_button->setCapInsets(Rect(3, 3, 5, 5));
	_quit_button->addClickEventListener([=](Ref*) {
		//_select_user1_team_red[0]->setBright(true);
		CCLOG("!!!!!!!!!click Quit \n");
	});

	//----------------------------- champ && chat select -------------------------------//
	
	_champ_and_chat_panel = _team_panel->getChildByTag(1060);
	_champ_bg_image = dynamic_cast<ui::ImageView*>(_champ_and_chat_panel->getChildByTag(1061));

	_champ_scroll = dynamic_cast<ui::ScrollView*>(_champ_and_chat_panel->getChildByTag(1062));
	_champ_scroll_image[0] = dynamic_cast<ui::ImageView*>(_champ_scroll->getChildByTag(1063));
	_champ_scroll_image[1] = dynamic_cast<ui::ImageView*>(_champ_scroll->getChildByTag(1064));
	
	_champ_in_scroll_panel1 = dynamic_cast<Node*>(_champ_scroll->getChildByTag(1084));

	for (int i = 0; i < 18; i++) {
		_champion_one[i] = dynamic_cast<ui::Button*>(_champ_in_scroll_panel1->getChildByTag(1085 + i));
		_champion_one[i]->setZoomScale(0.4f);
		_champion_one[i]->setPressedActionEnabled(true);
		_champion_one[i]->setScale9Enabled(true);
		_champion_one[i]->setCapInsets(Rect(3, 3, 5, 5));
		_champion_one[i]->addTouchEventListener(
			CC_CALLBACK_2(TeamSelectScene::championButtonEvent, this, "Assasin"));
	}
	
	_champ_in_scroll_panel2 = dynamic_cast<Node*>(_champ_scroll->getChildByTag(1065));

	for (int i = 0; i < 18; i++) {
		_champion_two[i] = dynamic_cast<ui::Button*>(_champ_in_scroll_panel2->getChildByTag(1066 + i));
		_champion_two[i]->setZoomScale(0.4f);
		_champion_two[i]->setPressedActionEnabled(true);
		_champion_two[i]->setScale9Enabled(true);
		_champion_two[i]->setCapInsets(Rect(3, 3, 5, 5));
		_champion_two[i]->addTouchEventListener(
			CC_CALLBACK_2(TeamSelectScene::championButtonEvent, this, "Cow"));
	}

	size_t idx = 0;
	rapidjson::Document json;
	util::readJSON("characters/images/user_character_images.json", json);

	for (rapidjson::Value::ValueIterator it = json.Begin(); it != json.End(); it++)
	{
		std::string name = it->GetString();
		std::string filename = "characters/images/" + name + ".png";
		_champion_one[idx]->loadTextureNormal(filename);
		_champion_one[idx]->addTouchEventListener(
			CC_CALLBACK_2(TeamSelectScene::championButtonEvent, this, name));
		_champion_one[idx++]->loadTexturePressed(filename);
	}


	_chat_listview = dynamic_cast<ui::ListView*>(_champ_and_chat_panel->getChildByTag(1106));
	_chat_listview->setScrollBarPositionFromCornerForVertical(Vec2(10, 7));
	_chat_listview->setScrollBarColor(Color3B::WHITE);
	_chat_listview->setScrollBarAutoHideEnabled(false);
	_chat_listview->setScrollBarWidth(15);
	_chat_listview->setScrollBarOpacity(100);
	_chat_listview->setBounceEnabled(true);
	_chat_listview->removeAllChildrenWithCleanup(true);


	_chat_list[0] = dynamic_cast<ui::Text*>(_chat_listview->getChildByTag(1107));
	_chat_list[1] = dynamic_cast<ui::Text*>(_chat_listview->getChildByTag(1108));
	_chat_list[2] = dynamic_cast<ui::Text*>(_chat_listview->getChildByTag(1109));
	_chat_list[3] = dynamic_cast<ui::Text*>(_chat_listview->getChildByTag(1110));
	_chat_list[4] = dynamic_cast<ui::Text*>(_chat_listview->getChildByTag(1111));

	
	auto text_field = dynamic_cast<ui::TextField*>(_champ_and_chat_panel->getChildByTag(1112));
	text_field->setTouchEnabled(false);
	text_field->setVisible(false);

	_chat_text_field = CCTextFieldTTF::textFieldWithPlaceHolder(
		"messages", text_field->getContentSize(), TextHAlignment::LEFT, "Arial", 20);
	text_field->getParent()->addChild(_chat_text_field);
	_chat_text_field->setPosition(text_field->getPosition());
	_chat_text_field->setAnchorPoint(text_field->getAnchorPoint());
	_chat_text_field->setMaxLineWidth(60);

	_chat_text_field->detachWithIME();
	_activate_text_field = false;


	//_game_panel = rootNode->getChildByTag(1);
	//_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, _character_panel);
	
	// keyboard event
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(TeamSelectScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(TeamSelectScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	this->scheduleUpdate();

	return true;
}


void TeamSelectScene::setUserEventAlive(int num) {

	_select_user_team_red[num]->setZoomScale(0.4f);
	_select_user_team_red[num]->setPressedActionEnabled(true);
	_select_user_team_red[num]->setScale9Enabled(true);
	_select_user_team_red[num]->setCapInsets(Rect(3, 3, 5, 5));
	
	_select_user_team_red[num]->
		addTouchEventListener(CC_CALLBACK_2(TeamSelectScene::teamButtonEvent, this, TEAM_RED));
		
	_select_user_team_blue[num]->setZoomScale(0.4f);
	_select_user_team_blue[num]->setPressedActionEnabled(true);
	_select_user_team_blue[num]->setScale9Enabled(true);
	_select_user_team_blue[num]->setCapInsets(Rect(3, 3, 5, 5));

	_select_user_team_blue[num]->
		addTouchEventListener(CC_CALLBACK_2(TeamSelectScene::teamButtonEvent, this, TEAM_BLUE));

	for (int i = 0; i < 6; i++) {
		if (i != num) {
			_select_user_team_red[i]->setTouchEnabled(false);
			_select_user_team_blue[i]->setTouchEnabled(false);
		}
	}
}


void TeamSelectScene::setRoomChatMessage(const std::string& name, const std::string& chat_message)
{
	static int msg_cnt = 0;
	auto newText = ui::Text::create();
	newText->setFontSize(20);
	newText->setString(name + " : " + chat_message);
	
	_chat_listview->pushBackCustomItem(newText);
	_chat_listview->scrollToItem(msg_cnt, Vec2::ANCHOR_MIDDLE, Vec2::ANCHOR_MIDDLE);

	if (msg_cnt >= MAX_MESSAGES)
	{
		_chat_listview->removeItem(0);
	}
	else
	{
		msg_cnt++;
	}
}


void TeamSelectScene::update(float delta)
{
	GameMessage temp_msg;
	while (PacketFactory::getInstance().getQ().pop(temp_msg))
	{
		PacketFactory::getInstance().decodeByType(temp_msg);
	}
}

void TeamSelectScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	//if enter key pressed, then user can write messages
	//if again they can write messages.
	if (keyCode == EventKeyboard::KeyCode::KEY_ENTER)
	{
		_activate_text_field = !_activate_text_field;

		if (_activate_text_field)
		{
			_chat_text_field->attachWithIME();
			_chat_text_field->setString("");
		}
		else
		{
			GameClientManager::getInstance().send(
				PacketFactory::getInstance().createPacketRoomChatMessageToServer(
					GameClientManager::getInstance().getGameClient()->getId(),
					_chat_text_field->getString()));

			_chat_text_field->setString("");
			_chat_text_field->detachWithIME();
		}
	}

	if (_activate_text_field)
		return;

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_ENTER:

		break;

	}
}

void TeamSelectScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{

}


void TeamSelectScene::readyButtonEvent(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
	{
		clickedReadyFlag();

		int user = getSlotNumber();

		GameClientManager::getInstance().send(
			PacketFactory::getInstance().createPacketClickedReady(
				GameClientManager::getInstance().getGameClient()->getId(), getReadyFlag()));
	}
	break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void TeamSelectScene::teamButtonEvent(Ref *pSender, ui::Widget::TouchEventType type, int team)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:

		GameClientManager::getInstance().getGameClient()->setTeam(team);
		GameClientManager::getInstance().getGameClient()->send(
			PacketFactory::getInstance().createPacketSelectTeam(
				GameClientManager::getInstance().getGameClient()->getId(), team));
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void TeamSelectScene::championButtonEvent(
	Ref *pSender,
	ui::Widget::TouchEventType type,
	const std::string& cname)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
		GameClientManager::getInstance().getGameClient()->setCharacterName(cname);
		GameClientManager::getInstance().getGameClient()->send(
			PacketFactory::getInstance().createPacketSelectCharacter(
				GameClientManager::getInstance().getGameClient()->getId(), cname));
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

cocos2d::ui::Text* TeamSelectScene::getUserIdText(int slot)
{
	CCASSERT(0 <= slot && slot < SLOT_SIZE, "slot is out of range!");

	return _user_id_text[slot];
}

cocos2d::ui::ImageView* TeamSelectScene::getUserCheckRed(int slot)
{
	CCASSERT(0 <= slot && slot < SLOT_SIZE, "slot is out of range!");

	return _select_user_check_red[slot];
}

cocos2d::ui::ImageView* TeamSelectScene::getUserCheckBlue(int slot)
{
	CCASSERT(0 <= slot && slot < SLOT_SIZE, "slot is out of range!");

	return _select_user_check_blue[slot];
}

cocos2d::ui::ImageView* TeamSelectScene::getUserChampImage(int slot)
{
	CCASSERT(0 <= slot && slot < SLOT_SIZE, "slot is out of range!");

	return _select_user_champ_image[slot];
}

cocos2d::ui::ImageView* TeamSelectScene::getSelectUserCheckChamp(int slot)
{
	CCASSERT(0 <= slot && slot < SLOT_SIZE, "slot is out of range!");

	return _select_user_check_champ[slot];
}


void TeamSelectScene::setSlotNumber(int slot_number)
{
	_slot_number = slot_number;
}

int TeamSelectScene::getSlotNumber() const
{
	return _slot_number;
}


void TeamSelectScene::clickedReadyFlag()
{
	if(_ready_flag == CLICKED_READY)
		_ready_flag = CLICKED_NOT_READY;
	else
		_ready_flag = CLICKED_READY;
}

int TeamSelectScene::getReadyFlag() const
{
	return _ready_flag;
}