#pragma once

#include <memory>
#include <string>
#include "AbstCharacter.h"

//---------------------------------------------------------------------------
//
//  Name:   UserCharacter.h
//
//  Desc:   This class is describe the character which is controlled by user.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//---------------------------------------------------------------------------



class UserCharacter : public AbstCharacter {
public:

	UserCharacter(
		cocos2d::Layer* const	view_layer,
		const std::string&		cname,
		GameWorld* const		world,
		const cocos2d::Vec2&	position,
		const cocos2d::Vec2&	margin,
		double					rotation,
		double					mass,
		double					max_force,
		double					max_speed,
		double					max_turn_rate,
		double					bounding_radius,
		int						team,
		int						max_health,
		int						meleeAttackPower,
		int						gold,
		int						worth);

	void fireWeapon(cocos2d::Vec2 pos);

	virtual ~UserCharacter() override;

	//virtual functions
	virtual void	update(double time_elapsed) override;

	virtual bool	handleMessage(const Telegram& msg) override;

private:
	UserCharacter(const UserCharacter&) = delete; // no copies
	UserCharacter& operator=(const UserCharacter&) = delete; // no self-assignments
	UserCharacter() = delete;

};




