#include "LoginScene.h"
#include "AppMacros.h"
#include "ParameterLoader.h"
#include "GlobalValues.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Network\GameClient\GameClientManager.h"
#include "Network\Packet\PacketFactory.h"

#include "SimpleAudioEngine.h"

USING_NS_CC;


Scene* LoginScene::scene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	LoginScene *layer = LoginScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	//------------------------User Interface Setting-----------------------------//

	//auto director = Director::getInstance();
	//auto glview = director->getOpenGLView();
	//glview->setFrameSize(500, 500);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(LoginScene::menuCloseCallback, this));

	closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));


	FileUtils::getInstance()->addSearchPath("UI");
	auto rootNode = CSLoader::createNode("LoginScene.csb");


	_login_panel = rootNode->getChildByTag(180);

	// join button
	_join_button = dynamic_cast<ui::Button*>(_login_panel->getChildByTag(184));
	_join_button->addTouchEventListener(CC_CALLBACK_2(LoginScene::buttonEvent, this));
	_join_button->setScale9Enabled(true);
	_join_button->setTitleFontName("courbd.ttf");

	//login text field
	_login_bg = dynamic_cast<ui::ImageView*>(_login_panel->getChildByTag(181));
	_login_text_bg = dynamic_cast<ui::ImageView*>(_login_panel->getChildByTag(187));
	_login_text_field = dynamic_cast<ui::TextField*>(_login_panel->getChildByTag(185));
	_login_text_field->addEventListener(CC_CALLBACK_2(LoginScene::textFieldEvent, this));
	_login_text_field->setString("");
	_login_text_field->setPlaceHolder("ID");
	_login_text_field->setFontName("courbd.ttf");
	_login_text_field->setTextColor(Color4B::WHITE);

	addChild(rootNode);


	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	this->scheduleUpdate();

	return true;
}


void LoginScene::menuCloseCallback(Ref* sender)
{
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}


void LoginScene::buttonEvent(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		//_displayValueLabel->setString(StringUtils::format("Touch Down"));
		break;

	case ui::Widget::TouchEventType::MOVED:
		//_displayValueLabel->setString(StringUtils::format("Touch Move"));
		break;

	case ui::Widget::TouchEventType::ENDED:
	{
		// first time of making client
		// causes PacketAssignID from server
		GameClientManager::getInstance().getGameClient()->
			setClientName(_login_text_field->getString().c_str());

		auto background_music = CocosDenshion::SimpleAudioEngine::getInstance();
		background_music->playBackgroundMusic("selectSceneSound.mp3", false);

		auto director = Director::getInstance();
		director->popScene();
	}
	break;

	case ui::Widget::TouchEventType::CANCELED:
		//_displayValueLabel->setString(StringUtils::format("Touch Cancelled"));
		break;

	default:
		break;
	}
}

void LoginScene::textFieldEvent(Ref *pSender, ui::TextField::EventType type)
{
	switch (type)
	{
	case ui::TextField::EventType::ATTACH_WITH_IME:
	{
		ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);
		textField->setColor(cocos2d::Color3B::GRAY);
		//textField->setString(StringUtils::format("attach with IME"));
	}
	break;

	case ui::TextField::EventType::DETACH_WITH_IME:
	{
		//ui::TextField* textField = dynamic_cast<ui::TextField*>(pSender);
		//textField->setString(StringUtils::format("detach with IME"));
	}
	break;

	case ui::TextField::EventType::INSERT_TEXT:
		break;

	case ui::TextField::EventType::DELETE_BACKWARD:
		break;

	default:
		break;
	}
}

void LoginScene::update(float delta)
{
	// loop for process client read messages
	GameMessage temp_msg;
	while (PacketFactory::getInstance().getQ().pop(temp_msg))
	{
		PacketFactory::getInstance().decodeByType(temp_msg);
	}
}