#pragma once

#include "cocos2d.h"
#include <string>

class RenderUtil 
{
public:

	enum rendering_type
	{
		MAIN = 0,
		BOUNDING_BOX = 1,
		SHADOW = 2,
		CUSTOM_SHAPE = 3,
		LABEL = 4,
		CELL_BOX = 5,
		CELL_LABEL = 6
	};

	static void shadowSetting(
		cocos2d::Layer* const view_layer,
		cocos2d::Node* const object,
		cocos2d::Sprite* const shadow,
		cocos2d::Vec2 pos);

	static float calculateSkew(cocos2d::Node *node, cocos2d::Vec2 light_position);

	static void labelSetting(
		cocos2d::Node* const object,
		cocos2d::Label* const label,
		const cocos2d::Vec2& pos,
		const cocos2d::Color3B& color = cocos2d::Color3B::WHITE);

	static void updateLabel(
		cocos2d::Label* const label,
		const std::string& str);
};