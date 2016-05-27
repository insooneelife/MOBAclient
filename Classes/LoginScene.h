#ifndef __WAITINGROOM_SCENE_H__
#define __WAITINGROOM_SCENE_H__

#include <memory>
#include "cocos2d.h"

#include "ui\UIImageView.h"
#include "ui\UIButton.h"
#include "ui\UIText.h"
#include "ui\UILoadingBar.h"
#include "ui\UIWidget.h"
#include "ui\UITextField.h"
#include "ui\UIListView.h"


class TeamSelectRoom;

class LoginScene : public cocos2d::Layer
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

	// implement the "static create()" method manually
	CREATE_FUNC(LoginScene);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* sender);

	void buttonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	void textFieldEvent(cocos2d::Ref *pSender, cocos2d::ui::TextField::EventType type);

	//schedule processes of gameworld or other events
	virtual void update(float fDelta) override;

private:
	cocos2d::Node*				_login_panel;
	cocos2d::ui::Button*		_join_button;

	cocos2d::ui::ImageView*		_login_bg;
	cocos2d::ui::ImageView*		_login_text_bg;
	cocos2d::TextFieldTTF*		_login_id_text;
	cocos2d::ui::TextField*		_login_text_field;
};

#endif 
