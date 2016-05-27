#include <list>
#include <string>
#include <tuple>
#include "Character.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.hpp"
#include "Geometry\Geometry.h"
#include "DebugRenderer.h"
#include "Database.h"

#include <sstream>

USING_NS_CC;

Character::Character(
	Layer* const			view_layer,
	GameWorld* const		world,
	unsigned int			id,
	const std::string&		cname,
	const cocos2d::Vec2&	position,
	const cocos2d::Vec2&	margin,
	double					bounding_radius)
	:
	BaseGameEntity(
		world,
		new geometry::Circle(position, bounding_radius),
		id,
		CHARACTER,
		cname,
		bounding_radius,
		position,
		margin),
	_motion(new Motion(cname, Prm.CharacterFolderName, this, position, 0)),
	_motion_name("Neutral")
{
	//--------------------------Static Data Setting-------------------------------//
	
	_attack_range = std::get<6>(DB.getCharacterData()[cname]);
	_view_range = std::get<7>(DB.getCharacterData()[cname]);
	_gold_worth = std::get<10>(DB.getCharacterData()[cname]);
	_exp_worth = std::get<11>(DB.getCharacterData()[cname]);


	//--------------------------Rendering Setting-------------------------------//
	_now_sprite = Sprite::create();
	_now_sprite->setCameraMask(1 << 2);

	float min_x = _now_sprite->getBoundingBox().getMinX();
	float min_y = _now_sprite->getBoundingBox().getMinY();
	float max_x = _now_sprite->getBoundingBox().getMaxX();
	float max_y = _now_sprite->getBoundingBox().getMaxY();

	float textureX = max_x - min_x;
	float textureY = max_y - min_y;

	_now_sprite->setPosition(position);
	_now_sprite->setGlobalZOrder(util::zOrdering(position.x, position.y));
	_now_sprite->setLocalZOrder(util::zOrdering(position.x, position.y));

	_shadow_sprite = Sprite::create();
	_shadow_sprite->setCameraMask(1 << 2);
	
	
	RenderUtil::shadowSetting(
		view_layer,
		_now_sprite,
		_shadow_sprite,
		_pos);

	_shadow_sprite->setPosition(_pos + Vec2(0, margin.y * _shadow_sprite->getScaleY()));

	view_layer->addChild(_now_sprite);
	_bounding_shape->render(Debug.getBoundingShapePanel());

	_main_label = Label::createWithTTF("", "arial.ttf", 11);
	_main_label->setVerticalAlignment(cocos2d::TextVAlignment::BOTTOM);
	_main_label->setHorizontalAlignment(cocos2d::TextHAlignment::RIGHT);
	_main_label->setAnchorPoint(Vec2(1, 1));
	_main_label->setVisible(true);
	_main_label->setCameraMask(1 << 2);

	RenderUtil::labelSetting(_now_sprite, _main_label, Vec2(0, 0));
}

Character::~Character()
{
	_shadow_sprite->removeFromParent();
	_main_label->removeFromParent();

	//_now_sprite must be deleted after child renderers.
	_now_sprite->removeFromParent();

	_motion.reset();
}



void Character::update(
	cocos2d::Vec2 pos, int frame, const std::string& motion_name,
	int health, int  mana, int  damage, int mdamage, int attack_speed, int move_speed)
{
	setPos(pos);
	_motion->setFrame(frame);
	setMotionName(motion_name);
	_health = health;
	_mana = mana;
	_damage = damage;
	_magic_damage = mdamage;
	_attack_speed = attack_speed;
	_move_speed = move_speed;
}

void Character::render()
{
	_motion->render();
	_bounding_shape->render(Debug.getBoundingShapePanel());

	Debug.getViewCirclePanel()->drawCircle(_pos, _view_range, M_PI * 2, 40, false, Color4F::YELLOW);
}

bool Character::handleMessage(const Telegram& telegram)
{
	switch (telegram.msg)
	{
	case MSG_PATH_READY:
	{
		//CCLOG("MSG_PATH_READY");
		return true;
	}
	case MSG_NOPATH_AVAILABLE:
	{
		//CCLOG("MSG_NOPATH_AVAILABLE");
	}
	break;
	}

	return false; 
}


//getters & setters

const std::unique_ptr<Character::Motion>& Character::getMotion() const
{
	return _motion;
}

Sprite* Character::getNowSprite() const
{
	return _now_sprite;
}

Sprite* Character::getShadowSprite() const
{
	return _shadow_sprite;
}

Label* Character::getMainLabel() const
{
	return _main_label;
}

const std::string& Character::getMotionName() const
{
	return _motion_name;
}

void Character::setMotionName(const std::string& motion)
{
	_motion_name = motion;
}

int Character::getHealth() const
{
	return _health;
}
void Character::setHealth(int health)
{
	_health = health;
}

int Character::getMana() const
{
	return _mana;
}
void Character::setMana(int mana)
{
	_mana = mana;
}

int Character::getDamage() const
{
	return _damage;
}
void Character::setDamage(int damage)
{
	_damage = damage;
}

int Character::getMagicDamage() const
{
	return _magic_damage;
}
void Character::setMagicDamage(int magic_damage)
{
	_magic_damage = magic_damage;
}

int Character::getAttackSpeed() const
{
	return _attack_speed;
}
void Character::setAttackSpeed(int attack_speed)
{
	_attack_speed = attack_speed;
}

int Character::getMoveSpeed() const
{
	return _move_speed;
}
void Character::setMoveSpeed(int move_speed)
{
	_move_speed = move_speed;
}

int Character::getAttackRange() const
{
	return _attack_range;
}

void Character::setAttackRange(int range)
{
	_attack_range = range;
}

int Character::getViewRange() const
{
	return _view_range;
}

void Character::setViewRange(int range)
{
	_view_range = range;
}


int Character::getGoldWorth() const
{
	return _gold_worth;
}

void Character::setGoldWorth(int worth)
{
	_gold_worth = worth;
}

int Character::getExpWorth() const
{
	return _exp_worth;
}

void Character::setExpWorth(int worth)
{
	_exp_worth = worth;
}