#include <algorithm>
#include "AbstWeaponSystem.h"
#include "BaseCastingStrategy.h"
#include "AbstCharacter.h"
#include "Util.h"
#include "GameWorld.h"
#include "Geometry\Transformations.h"

USING_NS_CC;

//------------------------- ctor ----------------------------------------------
//-----------------------------------------------------------------------------
AbstWeaponSystem::AbstWeaponSystem(
	AbstCharacter* owner,
	double reactionTime,
	double aim_accuracy,
	double aim_persistance) 
	:
	_owner(owner),
	_reaction_time(reactionTime),
	_aim_accuracy(aim_accuracy),
	_aim_persistance(aim_persistance)
{
	initialize();
}

//------------------------- dtor ----------------------------------------------
//-----------------------------------------------------------------------------
AbstWeaponSystem::~AbstWeaponSystem()
{}

//------------------------------ initialize -----------------------------------
//
//  initializes the weapons
//-----------------------------------------------------------------------------
void AbstWeaponSystem::initialize()
{
	_weapon_map.clear();

	//set up the container
	_current_weapon = new BaseCastingStrategy(_owner);
	_weapon_map[BaseGameEntity::TYPE_ROCKET_LAUNCHER].reset(_current_weapon);
}

//-------------------------------- selectWeapon -------------------------------
//
//-----------------------------------------------------------------------------
void AbstWeaponSystem::selectWeapon()
{

}

//------------------------- getWeaponFromInventory ----------------------------
//
//  returns a pointer to any matching weapon.
//
//  returns a null pointer if the weapon is not present
//-----------------------------------------------------------------------------
CastingStrategy* AbstWeaponSystem::getWeaponFromInventory(int weapon_type)
{
	return _weapon_map[weapon_type].get();
}

//--------------------------- changeWeapon ------------------------------------
void AbstWeaponSystem::changeWeapon(unsigned int type)
{
	CastingStrategy* w = getWeaponFromInventory(type);

	if (w) _current_weapon = w;
}

//--------------------------- takeAimAndShoot ---------------------------------
//
//  this method aims the bots current weapon at the target (if there is a
//  target) and, if aimed correctly, fires a round
//-----------------------------------------------------------------------------
void AbstWeaponSystem::takeAimAndShoot()const
{
	//aim the weapon only if the current target is shootable or if it has only
	//very recently gone out of view (this latter condition is to ensure the 
	//weapon is aimed at the target even if it temporarily dodges behind a wall
	//or other cover)
	if (_owner->getTargetSys()->isTargetAttackable() ||
		(_owner->getTargetSys()->getTimeTargetHasBeenOutOfView() <
			_aim_persistance))
	{
		//the position the weapon will be aimed at
		Vec2 aiming_pos = _owner->getTargetSys()->getTarget()->getPos();

		//if the current weapon is not an instant hit type gun the target position
		//must be adjusted to take into account the predicted movement of the 
		//target
		if (getCurrentWeapon()->getType() == BaseGameEntity::TYPE_ROCKET_LAUNCHER)
		{
			aiming_pos = predictFuturePositionOfTarget();

			//if the weapon is aimed correctly, there is line of sight between the
			//bot and the aiming position and it has been in view for a period longer
			//than the bot's reaction time, shoot the weapon
			if (_owner->rotateFacingTowardPosition(aiming_pos) &&
				(_owner->getTargetSys()->getTimeTargetHasBeenVisible() > _reaction_time))
			{
				addNoiseToAim(aiming_pos);

				getCurrentWeapon()->shootAt(aiming_pos);
			}
		}

		//no need to predict movement, aim directly at target
		else
		{
			//if the weapon is aimed correctly and it has been in view for a period
			//longer than the bot's reaction time, shoot the weapon
			if (_owner->rotateFacingTowardPosition(aiming_pos) &&
				(_owner->getTargetSys()->getTimeTargetHasBeenVisible() >
					_reaction_time))
			{
				addNoiseToAim(aiming_pos);

				getCurrentWeapon()->shootAt(aiming_pos);
			}
		}

	}

	//no target to shoot at so rotate facing to be parallel with the bot's
	//heading direction
	else
	{
		_owner->rotateFacingTowardPosition(_owner->getPos() + _owner->getHeading());
	}
}

//---------------------------- addNoiseToAim ----------------------------------
//
//  adds a random deviation to the firing angle not greater than _aim_accuracy 
//  rads
//-----------------------------------------------------------------------------
void AbstWeaponSystem::addNoiseToAim(Vec2& aiming_pos)const
{
	Vec2 toPos = aiming_pos - _owner->getPos();

	Vec2DRotateAroundOrigin(toPos, util::genRand(-_aim_accuracy, _aim_accuracy));

	aiming_pos = toPos + _owner->getPos();
}

//-------------------------- predictFuturePositionOfTarget --------------------
//
//  predicts where the target will be located in the time it takes for a
//  projectile to reach it. This uses a similar logic to the Pursuit steering
//  behavior.
//-----------------------------------------------------------------------------
Vec2 AbstWeaponSystem::predictFuturePositionOfTarget() const
{
	double max_speed = getCurrentWeapon()->getMaxProjectileSpeed();

	//if the target is ahead and facing the agent shoot at its current pos
	Vec2 to_enemy = _owner->getTargetSys()->getTarget()->getPos() - _owner->getPos();

	//the lookahead time is proportional to the distance between the enemy
	//and the pursuer; and is inversely proportional to the sum of the
	//agent's velocities
	double look_ahead_time = to_enemy.getLength() /
		(max_speed + _owner->getTargetSys()->getTarget()->getMaxSpeed());

	//return the predicted future position of the enemy
	return _owner->getTargetSys()->getTarget()->getPos() +
		_owner->getTargetSys()->getTarget()->getVelocity() * look_ahead_time;
}



//---------------------------- shootAt ----------------------------------------
//
//  shoots the current weapon at the given position
//-----------------------------------------------------------------------------
void AbstWeaponSystem::shootAt(cocos2d::Vec2 pos)const
{
	getCurrentWeapon()->shootAt(pos);
}
