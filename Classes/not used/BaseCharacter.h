#pragma once

#include <memory>
#include <string>
#include "AbstCharacter.h"

//------------------------------------------------------------------------
//
//  Name:   BaseCharacter.h
//
//  Desc:   This class is abstract class for game agent witch has
//			goal system				(GoalThink),
//			movements				(SteeringBehavior),
//			animations				(Motion),
//			rendering				(cocos2d Sprite)
//			and navigation system	(PathPlanner).
//			This class also has reference for GameWorld,
//			so it can get interact with other entities.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//--------------------------------------------------------------------------

class BaseCharacter : public AbstCharacter {
public:

	BaseCharacter(
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
		std::queue<cocos2d::Vec2> _pathQ,
		int						meleeAttackPower,
		int						gold,
		int						worth);

	virtual ~BaseCharacter() override;

	//virtual functions
	virtual void	update(double time_elapsed) override;

	virtual bool	handleMessage(const Telegram& msg) override;

private:
	BaseCharacter(const BaseCharacter&) = delete; // no copies
	BaseCharacter& operator=(const BaseCharacter&) = delete; // no self-assignments
	BaseCharacter() = delete;

};




