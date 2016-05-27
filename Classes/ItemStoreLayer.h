#ifndef __ITEMSTORE_LAYER_H__
#define __ITEMSTORE_LAYER_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
#include "ui\UIImageView.h"
#include "ui\UITextField.h"
#include "ui\UIScrollView.h"
#include "ui\UIText.h"


class ItemStoreLayer : public cocos2d::Layer
{
public:
	enum { NONE, BUY, SELL };

	// there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Layer* createLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(ItemStoreLayer);


	void itemButtonEvent(
		cocos2d::Ref *pSender,
		cocos2d::ui::Widget::TouchEventType type,
		int i, int j);
	
	void userItemButtonEvent(
		Ref *pSender,
		cocos2d::ui::Widget::TouchEventType type);

	void itemSellButtonEvent(
		Ref *pSender,
		cocos2d::ui::Widget::TouchEventType type);

	void itemBuyButtonEvent(
		Ref *pSender,
		cocos2d::ui::Widget::TouchEventType type);

	void updateItemImage();


private:

	cocos2d::Node* _store_panel;

	//store item panel
	cocos2d::Node* _store_item_panel;
	
	cocos2d::ui::ScrollView* _store_item_scroll;
	cocos2d::Node* _scroll_view_item_line_penel[10];
	cocos2d::Node* _scroll_view_item_penel[10][6]; // y , x
	cocos2d::ui::Button* _scroll_item_button[10][6];
	cocos2d::ui::Text* _scroll_item_gold_text[10][6];




	//store user item panel
	cocos2d::Node* _store_user_item_panel;
	cocos2d::Node* _store_user_own_item_panel[6];
	cocos2d::ui::Button* _store_user_own_item_button[6];
	cocos2d::ui::Text* _store_user_own_item_gold_text[6];
	cocos2d::ui::Button* _store_user_item_sell_button;
	cocos2d::ui::Text* _store_user_own_gold_text;



	//


	//


	//store  item info panel
	cocos2d::Node* _item_info_panel;




	//store select item info panel
	cocos2d::Node* _store_select_item_info_panel;
	cocos2d::ui::ImageView* _store_select_item_img;
	cocos2d::ui::Text* _store_select_item_name;
	cocos2d::ui::Text* _store_select_item_price;
	cocos2d::ui::Button* _store_select_item_buy_button;
	cocos2d::ui::Text* _store_select_item_info;





};

#endif // __HELLOWORLD_SCENE_H__
