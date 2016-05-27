#ifndef __OPTION_LAYER_H__
#define __OPTION_LAYER_H__

#include "cocos2d.h"
#include "ui\UIButton.h"
#include "ui\UIImageView.h"
#include "ui\UITextField.h"
#include "ui\UIScrollView.h"
#include "ui\UIText.h"
#include "ui\UICheckBox.h"

class OptionLayer : public cocos2d::Layer
{
public:
	enum Check
	{
		BOUNDING_BOX, CUSTOM_SHAPE, BOUNDING_SHAPE,
		VIEW_CIRCLE, GRAPH, PATH_LINE, CELL_SPACE,
		MAIN_LABEL, HEALTH, GOAL_LABEL, MEMORY_LABEL, DEFAULT
	};
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Layer* createLayer();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(OptionLayer);

	void selectedEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type,int checknum);


private:

	cocos2d::Node* _render_setting_panel;

	cocos2d::Node* _bounding_box_check_panel;
	cocos2d::ui::CheckBox* _bounding_box_check_box;
	cocos2d::ui::Text* _bounding_box_text;
	
	cocos2d::Node* _custom_shape_check_panel;
	cocos2d::ui::CheckBox* _custom_shape_check_box;
	cocos2d::ui::Text* _custom_shape_text;

	cocos2d::Node* _bounding_shape_check_panel;
	cocos2d::ui::CheckBox* _bounding_shape_check_box;
	cocos2d::ui::Text* _bounding_shape_text;

	cocos2d::Node* _view_circle_check_panel;
	cocos2d::ui::CheckBox* _view_circle_check_box;
	cocos2d::ui::Text* _view_circle_text;

	cocos2d::Node* _graph_check_panel;
	cocos2d::ui::CheckBox* _graph_check_box;
	cocos2d::ui::Text* _graph_text;

	cocos2d::Node* _path_line_check_panel;
	cocos2d::ui::CheckBox* _path_line_check_box;
	cocos2d::ui::Text* _path_line_text;

	cocos2d::Node* _cell_space_check_panel;
	cocos2d::ui::CheckBox* _cell_space_check_box;
	cocos2d::ui::Text* _cell_space_text;

	cocos2d::Node* _main_label_check_panel;
	cocos2d::ui::CheckBox* _main_label_check_box;
	cocos2d::ui::Text* _main_label_text;

	cocos2d::Node* _health_check_panel;
	cocos2d::ui::CheckBox* _health_check_box;
	cocos2d::ui::Text* _health_text;

	cocos2d::Node* _goal_label_check_panel;
	cocos2d::ui::CheckBox* _goal_label_check_box;
	cocos2d::ui::Text* _goal_label_text;

	cocos2d::Node* _memory_label_check_panel;
	cocos2d::ui::CheckBox* _memory_label_check_box;
	cocos2d::ui::Text* _memory_label_text;

	cocos2d::Node* _empty_check_panel;
	cocos2d::ui::CheckBox* _empty_check_box;
	cocos2d::ui::Text* _empty_label_text;


	void changeParamByCheckBox(int check, bool on_off);

};

#endif // __HELLOWORLD_SCENE_H__
