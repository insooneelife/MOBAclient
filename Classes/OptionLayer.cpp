#include "OptionLayer.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <string>
#include "DebugRenderer.h"
USING_NS_CC;

using namespace cocostudio::timeline;


Layer* OptionLayer::createLayer()
{

    // 'layer' is an autorelease object
    auto layer = OptionLayer::create();

    // return the scene
    return layer;
}

// on "init" you need to initialize your instance
bool OptionLayer::init()
{
    
    if ( !Layer::init() )
    {
        return false;
    }
    
	FileUtils::getInstance()->addSearchPath("UI");
    auto rootNode = CSLoader::createNode("OptionLayer.csb");
	addChild(rootNode);
	rootNode->setTag(0);

	rootNode->setPosition(cocos2d::Vec2(850, 250));

	_render_setting_panel = rootNode->getChildByTag(528);

	_bounding_box_check_panel = _render_setting_panel->getChildByTag(530);
	_bounding_box_check_box = dynamic_cast<ui::CheckBox*>(_bounding_box_check_panel->getChildByTag(531));
	_bounding_box_check_box->setSelected(false);
	_bounding_box_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, BOUNDING_BOX));
	_bounding_box_text = dynamic_cast<ui::Text*>(_bounding_box_check_panel->getChildByTag(532));


	_custom_shape_check_panel = _render_setting_panel->getChildByTag(533);
	_custom_shape_check_box = dynamic_cast<ui::CheckBox*>(_custom_shape_check_panel->getChildByTag(534));
	_custom_shape_check_box->setSelected(false);
	_custom_shape_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, CUSTOM_SHAPE));
	_custom_shape_text = dynamic_cast<ui::Text*>(_custom_shape_check_panel->getChildByTag(535));


	_bounding_shape_check_panel = _render_setting_panel->getChildByTag(536);
	_bounding_shape_check_box = dynamic_cast<ui::CheckBox*>(_bounding_shape_check_panel->getChildByTag(537));
	_bounding_shape_check_box->setSelected(false);
	_bounding_shape_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, BOUNDING_SHAPE));
	_bounding_shape_text = dynamic_cast<ui::Text*>(_bounding_shape_check_panel->getChildByTag(538));
	

	_view_circle_check_panel = _render_setting_panel->getChildByTag(539);
	_view_circle_check_box = dynamic_cast<ui::CheckBox*>(_view_circle_check_panel->getChildByTag(540));
	_view_circle_check_box->setSelected(false);
	_view_circle_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, VIEW_CIRCLE));
	_view_circle_text = dynamic_cast<ui::Text*>(_view_circle_check_panel->getChildByTag(541));


	_graph_check_panel = _render_setting_panel->getChildByTag(542);
	_graph_check_box = dynamic_cast<ui::CheckBox*>(_graph_check_panel->getChildByTag(543));
	_graph_check_box->setSelected(false);
	_graph_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, GRAPH));
	_graph_text = dynamic_cast<ui::Text*>(_graph_check_panel->getChildByTag(544));

	
	_path_line_check_panel = _render_setting_panel->getChildByTag(545);
	_path_line_check_box = dynamic_cast<ui::CheckBox*>(_path_line_check_panel->getChildByTag(546));
	_path_line_check_box->setSelected(false);
	_path_line_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, PATH_LINE));
	_path_line_text = dynamic_cast<ui::Text*>(_path_line_check_panel->getChildByTag(547));


	_cell_space_check_panel = _render_setting_panel->getChildByTag(548);
	_cell_space_check_box = dynamic_cast<ui::CheckBox*>(_cell_space_check_panel->getChildByTag(549));
	_cell_space_check_box->setSelected(false);
	_cell_space_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, CELL_SPACE));
	_cell_space_text = dynamic_cast<ui::Text*>(_cell_space_check_panel->getChildByTag(550));


	_main_label_check_panel = _render_setting_panel->getChildByTag(551);
	_main_label_check_box = dynamic_cast<ui::CheckBox*>(_main_label_check_panel->getChildByTag(552));
	_main_label_check_box->setSelected(false);
	_main_label_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, MAIN_LABEL));
	_main_label_text = dynamic_cast<ui::Text*>(_main_label_check_panel->getChildByTag(553));


	_health_check_panel = _render_setting_panel->getChildByTag(554);
	_health_check_box = dynamic_cast<ui::CheckBox*>(_health_check_panel->getChildByTag(555));
	_health_check_box->setSelected(false);
	_health_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, HEALTH));
	_health_text = dynamic_cast<ui::Text*>(_health_check_panel->getChildByTag(556));

	
	_goal_label_check_panel = _render_setting_panel->getChildByTag(557);
	_goal_label_check_box = dynamic_cast<ui::CheckBox*>(_goal_label_check_panel->getChildByTag(558));
	_goal_label_check_box->setSelected(false);
	_goal_label_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, GOAL_LABEL));
	_goal_label_text = dynamic_cast<ui::Text*>(_goal_label_check_panel->getChildByTag(559));


	_memory_label_check_panel = _render_setting_panel->getChildByTag(560);
	_memory_label_check_box = dynamic_cast<ui::CheckBox*>(_memory_label_check_panel->getChildByTag(561));
	_memory_label_check_box->setSelected(false);
	_memory_label_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, MEMORY_LABEL));
	_memory_label_text = dynamic_cast<ui::Text*>(_memory_label_check_panel->getChildByTag(562));


	_empty_check_panel = _render_setting_panel->getChildByTag(563);
	_empty_check_box = dynamic_cast<ui::CheckBox*>(_empty_check_panel->getChildByTag(564));
	_empty_check_box->setSelected(false);
	_empty_check_box->addEventListener(CC_CALLBACK_2(OptionLayer::selectedEvent, this, DEFAULT));
	_empty_label_text = dynamic_cast<ui::Text*>(_empty_check_panel->getChildByTag(565));

    return true;
}

void OptionLayer::selectedEvent(Ref* pSender, cocos2d::ui::CheckBox::EventType type , int check)
{
	switch (type)
	{
	case cocos2d::ui::CheckBox::EventType::SELECTED:
		changeParamByCheckBox(check, true);
		break;

	case cocos2d::ui::CheckBox::EventType::UNSELECTED:
		changeParamByCheckBox(check, false);
		break;

	default:
		break;
	}
}

void OptionLayer::changeParamByCheckBox(int check, bool on_off)
{
	switch (check)
	{
		case BOUNDING_BOX:
			Debug.getBoundingBoxPanel()->setVisible(on_off);
			break;

		case CUSTOM_SHAPE:
			Debug.getCustomShapePanel()->setVisible(on_off);
			break;

		case BOUNDING_SHAPE:
			Debug.getBoundingShapePanel()->setVisible(on_off);
			break;

		case VIEW_CIRCLE:
			Debug.getViewCirclePanel()->setVisible(on_off);
			break;

		case GRAPH:
			Debug.getGraphPanel()->setVisible(on_off);
			break;

		case PATH_LINE:
			Debug.getPathLinePanel()->setVisible(on_off);
			break;

		case CELL_SPACE:
			Debug.getCellBoxPanel()->setVisible(on_off);
			break;

		case MAIN_LABEL:
			break;

		case HEALTH:
			break;

		case GOAL_LABEL:
			break;

		case MEMORY_LABEL:
			break;

		case DEFAULT:
			break;

		default:
			break;
	}
}