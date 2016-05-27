#include <list>
#include <string>
#include "UserCharacter.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.hpp"
#include "Geometry\Geometry.h"
#include "DebugRenderer.h"

USING_NS_CC;


UserCharacter::UserCharacter(
	Layer* const			view_layer,
	GameWorld* const		world,
	unsigned int			id,
	const std::string&		cname,
	const cocos2d::Vec2&	position,
	const cocos2d::Vec2&	margin,
	double					bounding_radius,
	const std::string&		client_name)
	:
	Character(
		view_layer,
		world,
		id,
		cname,
		position,
		margin,
		bounding_radius),
	_client_name(client_name)
{

	//--------------------------"Rendering Setting-------------------------------//
	float length = 
		(_now_sprite->getTextureRect().getMaxX() - _now_sprite->getTextureRect().getMinX());
	
	_client_name_label = Label::createWithTTF("", "arial.ttf", 15);
	_client_name_label->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
	_client_name_label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
	_client_name_label->setAnchorPoint(Vec2(0.5, 1));
	_client_name_label->setString(_client_name);
	_client_name_label->setCameraMask(1 << 2);
	RenderUtil::labelSetting(_now_sprite, _client_name_label, Vec2(length * 0.2f, 0));
}

UserCharacter::~UserCharacter()
{
	_client_name_label->removeFromParent();
}

//getters & setters

const std::string& UserCharacter::getClientName() const
{
	return _client_name;
}

void UserCharacter::setClientName(const std::string& client_name)
{
	_client_name = client_name;
}

cocos2d::Label* UserCharacter::getClientNameLabel() const
{
	return _client_name_label;
}