#ifndef __ENDING_SCENE_H__
#define __ENDING_SCENE_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
#include "ui\UIImageView.h"
#include "ui\UITextField.h"
#include "ui\UIScrollView.h"
#include "ui\UIText.h"


class EndingScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(EndingScene);

	void win(const std::string& name, int team);


private:

	cocos2d::Node* _ending_panel;
	cocos2d::ui::Text* _red_win;
	cocos2d::ui::Text* _blue_win;


};

#endif // __HELLOWORLD_SCENE_H__
