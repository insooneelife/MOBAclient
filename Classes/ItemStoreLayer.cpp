#include "ItemStoreLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include <sstream>
#include "Database.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Layer* ItemStoreLayer::createLayer()
{
	// 'layer' is an autorelease object
	auto layer = ItemStoreLayer::create();

	// return the scene
	return layer;
}

// on "init" you need to initialize your instance
bool ItemStoreLayer::init()
{

	if (!Layer::init())
	{
		return false;
	}

	FileUtils::getInstance()->addSearchPath("UI");
	auto rootNode = CSLoader::createNode("ItemStoreLayer.csb");
	addChild(rootNode);
	rootNode->setTag(0);
	rootNode->setPosition(cocos2d::Vec2(325, 100));

	_store_panel = rootNode->getChildByTag(286);


	//store item panel
	_store_item_panel = _store_panel->getChildByTag(591);

	_store_item_scroll = dynamic_cast<ui::ScrollView*>(_store_item_panel->getChildByTag(592));
	_store_item_scroll->setScrollBarPositionFromCornerForVertical(Vec2(15, 7));
	_store_item_scroll->setScrollBarColor(Color3B::WHITE);
	_store_item_scroll->setScrollBarAutoHideEnabled(false);
	_store_item_scroll->setScrollBarWidth(10);
	_store_item_scroll->setScrollBarOpacity(100);


	std::string temp_item_name = "";


	//item line 0
	_scroll_view_item_line_penel[0] = _store_item_scroll->getChildByTag(610);

	//item line 0 - 0
	_scroll_view_item_penel[0][0] = _scroll_view_item_line_penel[0]->getChildByTag(594);
	_scroll_item_button[0][0] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][0]->getChildByTag(595));
	_scroll_item_button[0][0]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 0));
	_scroll_item_button[0][0]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][0] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][0]->getChildByTag(599));



	//item line 0 - 1
	_scroll_view_item_penel[0][1] = _scroll_view_item_line_penel[0]->getChildByTag(616);
	_scroll_item_button[0][1] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][1]->getChildByTag(617));
	_scroll_item_button[0][1]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 1));
	_scroll_item_button[0][1]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][1] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][1]->getChildByTag(620));


	//item line 0 - 2
	_scroll_view_item_penel[0][2] = _scroll_view_item_line_penel[0]->getChildByTag(621);
	_scroll_item_button[0][2] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][2]->getChildByTag(622));
	_scroll_item_button[0][2]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 2));
	_scroll_item_button[0][2]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][2] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][2]->getChildByTag(625));

	//item line 0 - 3
	_scroll_view_item_penel[0][3] = _scroll_view_item_line_penel[0]->getChildByTag(626);
	_scroll_item_button[0][3] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][3]->getChildByTag(627));
	_scroll_item_button[0][3]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 3));
	_scroll_item_button[0][3]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][3] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][3]->getChildByTag(630));

	//item line 0 - 4
	_scroll_view_item_penel[0][4] = _scroll_view_item_line_penel[0]->getChildByTag(631);
	_scroll_item_button[0][4] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][4]->getChildByTag(632));
	_scroll_item_button[0][4]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 4));
	_scroll_item_button[0][4]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][4] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][4]->getChildByTag(635));

	//item line 0 - 5
	_scroll_view_item_penel[0][5] = _scroll_view_item_line_penel[0]->getChildByTag(636);
	_scroll_item_button[0][5] = dynamic_cast<ui::Button*>(_scroll_view_item_penel[0][5]->getChildByTag(637));
	_scroll_item_button[0][5]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, 0, 5));
	_scroll_item_button[0][5]->setPressedActionEnabled(true);
	_scroll_item_gold_text[0][5] = dynamic_cast<ui::Text*>(_scroll_view_item_penel[0][5]->getChildByTag(640));


	int testbutton = 6;
	int idx = 646;
	for (int i = 1; i < 10; i++) 
	{
		_scroll_view_item_line_penel[i] = _store_item_scroll->getChildByTag(idx);
		idx++;

		for (int j = 0; j < 6; j++) 
		{
			_scroll_view_item_penel[i][j] = _scroll_view_item_line_penel[i]->getChildByTag(idx);
			idx++;
			_scroll_item_button[i][j] =
				dynamic_cast<ui::Button*>(_scroll_view_item_penel[i][j]->getChildByTag(idx));
			idx += 3;
			_scroll_item_button[i][j]->addTouchEventListener(
				CC_CALLBACK_2(ItemStoreLayer::itemButtonEvent, this, i, j));
			_scroll_item_button[i][j]->setPressedActionEnabled(true);

			_scroll_item_gold_text[i][j] = 
				dynamic_cast<ui::Text*>(_scroll_view_item_penel[i][j]->getChildByTag(idx));
			idx++;
		}
	}




	//store user item panel
	_store_user_item_panel = _store_panel->getChildByTag(956);

	idx = 957;

	for (int i = 0; i < 6; i++) {
		_store_user_own_item_panel[i] = _store_user_item_panel->getChildByTag(idx);
		idx++;
		_store_user_own_item_button[i] = dynamic_cast<ui::Button*>(_store_user_own_item_panel[i]->getChildByTag(idx));
		idx += 3;
		_store_user_own_item_button[i]->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::userItemButtonEvent, this));
		_store_user_own_item_button[i]->setPressedActionEnabled(true);
		_store_user_own_item_gold_text[i] = dynamic_cast<ui::Text*>(_store_user_own_item_panel[i]->getChildByTag(idx));
		idx += 2;
	}

	_store_user_item_sell_button = dynamic_cast<ui::Button*>(_store_user_item_panel->getChildByTag(1008));
	_store_user_item_sell_button->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemSellButtonEvent, this));
	_store_user_item_sell_button->setPressedActionEnabled(true);

	_store_user_own_gold_text = dynamic_cast<ui::Text*>(_store_user_item_panel->getChildByTag(1012));



	//store  item info panel
	_item_info_panel = _store_panel->getChildByTag(999);





	//store select item info panel
	_store_select_item_info_panel = _store_panel->getChildByTag(1000);

	_store_select_item_img = dynamic_cast<ui::ImageView*>(_store_select_item_info_panel->getChildByTag(1001));
	_store_select_item_name = dynamic_cast<ui::Text*>(_store_select_item_info_panel->getChildByTag(1002));
	_store_select_item_price = dynamic_cast<ui::Text*>(_store_select_item_info_panel->getChildByTag(1004));
	_store_select_item_buy_button = dynamic_cast<ui::Button*>(_store_select_item_info_panel->getChildByTag(1006));
	_store_select_item_buy_button->addTouchEventListener(CC_CALLBACK_2(ItemStoreLayer::itemBuyButtonEvent, this));
	_store_select_item_buy_button->setPressedActionEnabled(true);

	_store_select_item_info = dynamic_cast<ui::Text*>(_store_select_item_info_panel->getChildByTag(1007));
	_store_select_item_info->ignoreContentAdaptWithSize(false);


	return true;
}



void ItemStoreLayer::itemButtonEvent(
	Ref *pSender,
	ui::Widget::TouchEventType type,
	int i, int j)
{
	CCLOG("itemButtonEvent");
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
		//CCLOG("%d %d", i, j);
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}


void ItemStoreLayer::userItemButtonEvent(
	Ref *pSender,
	ui::Widget::TouchEventType type)
{
	CCLOG("userItemButtonEvent");
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
		//	GameClientManager::getInstance().getGameClient()->setCharacterName(cname);
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void ItemStoreLayer::itemSellButtonEvent(
	Ref *pSender,
	ui::Widget::TouchEventType type)
{
	CCLOG("itemSellButtonEvent");
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
		CCLOG("!!!!!!!!!press sell button \n");
		//	GameClientManager::getInstance().getGameClient()->setCharacterName(cname);
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void ItemStoreLayer::itemBuyButtonEvent(
	Ref *pSender,
	cocos2d::ui::Widget::TouchEventType type)
{
	CCLOG("itemBuyButtonEvent");
	switch (type)
	{
	case ui::Widget::TouchEventType::BEGAN:
		break;

	case ui::Widget::TouchEventType::MOVED:
		break;

	case ui::Widget::TouchEventType::ENDED:
		CCLOG("!!!!!!!!!press buy button \n");
		//	GameClientManager::getInstance().getGameClient()->setCharacterName(cname);
		break;

	case ui::Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void ItemStoreLayer::updateItemImage() 
{
	auto iter = DB.getInstance().getItemData();
	int idx = 0;

	CCLOG("!!!!!!!!!!!!!!!!!!adfafafdsafafasfasfaf");
	for (auto iter = std::begin(DB.getInstance().getItemData());
	iter!= std::end(DB.getInstance().getItemData()); iter++)
	{
		std::string name = iter->first;
		CCLOG("%s", name.c_str());
		_scroll_item_button[idx / 6][idx % 6]->loadTextureNormal("items/" + name + "/" + name + "_50x50.png");
		
		CCLOG("1");
		_scroll_item_button[idx / 6][idx % 6]->loadTexturePressed("items/" + name + "/" + name + "_50x50.png");
		
		CCLOG("2");

		std::stringstream ss;
		ss << std::get<7>(iter->second);
		_scroll_item_gold_text[idx / 6][idx % 6]->setString(ss.str());
		idx++;

		CCLOG("!!!!!!!!!!!!!!!!!!adfafafdsafafasfasfaf  %d",idx);
	}
}