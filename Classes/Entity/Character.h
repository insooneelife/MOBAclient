#pragma once

#include <memory>
#include <string>
#include <queue>
#include "BaseGameEntity.h"
#include "Motion.hpp"

//------------------------------------------------------------------------
//
//  Name:   Character.h
//
//  Desc:   
//
//  Author: Insub Im (insooneelife@naver.com)
//
//--------------------------------------------------------------------------

class GameWorld;

class Character : public BaseGameEntity 
{
public:
	typedef Motion<Character> Motion;

	Character(
		cocos2d::Layer* const	view_layer,
		GameWorld* const		world,
		unsigned int			id,
		const std::string&		cname,
		const cocos2d::Vec2&	position,
		const cocos2d::Vec2&	margin,
		double					bounding_radius);

	virtual ~Character() override;

	//getters && setters

	const std::unique_ptr<Motion>& getMotion() const;

	cocos2d::Sprite*			getNowSprite() const;
	cocos2d::Sprite*			getShadowSprite() const;
	cocos2d::Label*				getMainLabel() const;

	const std::string&			getMotionName() const;
	void						setMotionName(const std::string& motion);


	int		getHealth() const;
	void	setHealth(int health);

	int		getMana() const;
	void	setMana(int mana);

	int		getDamage() const;
	void	setDamage(int damage);

	int		getMagicDamage() const;
	void	setMagicDamage(int magic_damage);

	int		getAttackSpeed() const;
	void	setAttackSpeed(int attack_speed);

	int		getMoveSpeed() const;
	void	setMoveSpeed(int move_speed);

	int		getAttackRange() const;
	void	setAttackRange(int range);

	int		getViewRange() const;
	void	setViewRange(int range);

	int		getGoldWorth() const;
	void	setGoldWorth (int range);

	int		getExpWorth() const;
	void	setExpWorth(int range);


	//virtual functions
	void update(cocos2d::Vec2 pos, int frame, const std::string& motion_name,
		int health, int  mana, int  damage, int mdamage, int attack_speed, int move_speed);

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override;

protected:
	Character(const Character&) = delete; // no copies
	Character& operator=(const Character&) = delete; // no self-assignments
	Character() = delete;

	//Motion class for animate image frame.
	std::unique_ptr<Motion>					_motion;
	std::string								_motion_name;

	// dynamic data
	int		_health;
	int		_mana;
	int		_damage;
	int		_magic_damage;
	int		_attack_speed;
	int		_move_speed;

	// static data
	int		_attack_range;
	int		_view_range;
	int		_gold_worth;
	int		_exp_worth;


	//for rendering
	cocos2d::Sprite*			_now_sprite;
	cocos2d::Sprite*			_shadow_sprite;
	cocos2d::Label*				_main_label;

};


