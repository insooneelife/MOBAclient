#include "RenderUtil.h"
#include "GlobalValues.h"
#include "ParameterLoader.h"
#include "Util.h"

USING_NS_CC;

void RenderUtil::shadowSetting(
	Layer* const view_layer,
	Node* const object,
	Sprite* const shadow,
	Vec2 pos)
{
	view_layer->addChild(shadow);

	float shadow_move_y = 
		(object->getBoundingBox().getMaxY() - object->getBoundingBox().getMinY()) / 4;

	shadow->setGlobalZOrder(util::zOrdering(pos.x, pos.y + shadow_move_y));
	
	shadow->setTag(SHADOW);

	shadow->setVisible(Prm.RenderShadow);

	shadow->setSpriteFrame(((Sprite*)object)->getSpriteFrame());

	shadow->setPosition(pos);

	shadow->setAnchorPoint(Point(0.65f, 0.45f));

	shadow->setSkewX(-25);

	shadow->setColor(Color3B::BLACK);
	shadow->setOpacity(70);
}

float RenderUtil::calculateSkew(Node *node, Vec2 light_position)
{
	//http://www.cocos2d-x.org/wiki/User_Tutorial-Infinite_parallax_scrolling_with_primitive_shadows
	Node* object = node->getParent();
	float ED = light_position.y - object->getPosition().y;
	float EL = light_position.x - (node->getParent()->getPosition().x + node->getPosition().x);
	float DLE = atan(ED / EL);
	float DB = node->getContentSize().height * node->getScaleY() +
		object->getContentSize().height * object->getScaleY();
	float CB = tan(M_PI / 2 - DLE) * DB;
	float AB = node->getContentSize().height * node->getScaleY();
	float skew = 90.0 - CC_RADIANS_TO_DEGREES(atan(AB / CB));
	return skew;
}


void RenderUtil::labelSetting(
	Node* const object,
	Label* const label,
	const Vec2& pos,
	const Color3B& color)
{
	label->setGlobalZOrder(LABEL_ZORDER);
	label->setTag(LABEL);

	label->setColor(color);
	label->setPosition(pos);

	object->addChild(label, 1);
}

void RenderUtil::updateLabel(
	Label* const label,
	const std::string& str)
{
	label->setString(str);
}