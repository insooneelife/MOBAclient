#ifndef CASTING_STRATEGY_H
#define CASTING_STRATEGY_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   CastingStrategy.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   Base Weapon class for the raven project
//-----------------------------------------------------------------------------

#include <vector>

#include "cocos2d.h"
#include "timer/CrudeTimer.h"
#include "AbstCharacter.h"
#include "Util.h"


class  AbstCharacter;

class CastingStrategy
{
public:

	CastingStrategy(
		unsigned int	type_of_gun,
		double			RateOfFire,
		double			IdealRange,
		double			ProjectileSpeed,
		AbstCharacter* const OwnerOfGun);

	virtual ~CastingStrategy();

	//this method aims the weapon at the given target by rotating the weapon's
	//owner's facing direction (constrained by the bot's turning rate). It returns  
	//true if the weapon is directly facing the target.
	bool aimAt(cocos2d::Vec2 target) const;

	//The number of times a weapon can be discharges depends on its rate of fire.
	//This method returns true if the weapon is able to be discharged at the 
	//current time. (called from shootAt() )
	bool isReadyForNextShot();

	//this discharges a projectile from the weapon at the given target position
	//(provided the weapon is ready to be discharged... every weapon has its
	//own rate of fire)
	virtual void shootAt(cocos2d::Vec2 pos) = 0;

	//each weapon has its own shape and color
	virtual void render() = 0;


	//returns the maximum speed of the projectile this weapon fires
	double getMaxProjectileSpeed() const;

	unsigned int getType() const;
	double getIdealRange() const;

protected:

	//a weapon is always (in this game) carried by a bot
	AbstCharacter*	_owner;

	//an enumeration indicating the type of weapon
	unsigned int	_type;

	//the number of times this weapon can be fired per second
	double			_rate_of_fire;

	//the earliest time the next shot can be taken
	double			_time_next_available;

	//this is used to keep a local copy of the previous desirability score
	//so that we can give some feedback for debugging
	double			_last_desirability_score;

	//this is the prefered distance from the enemy when using this weapon
	double			_ideal_range;

	//the max speed of the projectile this weapon fires
	double			_max_skill_speed;

	//this is called when a shot is fired to update _time_next_available
	void			updateTimeWeaponIsNextAvailable();
};

#endif