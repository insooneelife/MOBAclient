#include "CastingStrategy.h"


CastingStrategy::CastingStrategy(
	unsigned int TypeOfGun,
	double        RateOfFire,
	double        IdealRange,
	double        ProjectileSpeed,
	AbstCharacter*   OwnerOfGun)
	:
	_type(TypeOfGun),
	_owner(OwnerOfGun),
	_rate_of_fire(RateOfFire),
	_last_desirability_score(0),
	_ideal_range(IdealRange),
	_max_skill_speed(ProjectileSpeed)
{
	_time_next_available = Clock.getCurrentTime();
}

CastingStrategy::~CastingStrategy() 
{}

///////////////////////////////////////////////////////////////////////////////
//------------------------ ReadyForNextShot -----------------------------------
//
//  returns true if the weapon is ready to be discharged
//-----------------------------------------------------------------------------
bool CastingStrategy::isReadyForNextShot()
{
	if (Clock.getCurrentTime() > _time_next_available)
	{
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
void CastingStrategy::updateTimeWeaponIsNextAvailable()
{
	_time_next_available = Clock.getCurrentTime() + 1.0 / _rate_of_fire;
}


//-----------------------------------------------------------------------------
bool CastingStrategy::aimAt(cocos2d::Vec2 target)const
{
	return _owner->rotateFacingTowardPosition(target);
}



double CastingStrategy::getMaxProjectileSpeed() const
{
	return _max_skill_speed; 
}

unsigned int CastingStrategy::getType() const
{
	return _type; 
}

double CastingStrategy::getIdealRange() const
{
	return _ideal_range; 
}
