#include "GoalBasicCasting.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"
#include "../RenderUtil.h"
#include "../Motion.h"
#include "../AbstWeaponSystem.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalBasicCasting::GoalBasicCasting(
	AbstCharacter* const bot,
	Label* const goal_label,
	Vec2 target)
	:
	Goal<AbstCharacter>(bot, GOAL_SEEK_TO_POS),
	_goal_label(goal_label),
	_target(target)
{}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalBasicCasting::activate()
{
	_status = ACTIVE;
	_owner->setVelocity(Vec2(0, 0));
	_owner->setHeading((_target - _owner->getPos()).getNormalized());
	_owner->setMotionDirect("Casting");
	_owner->getWeaponSys()->shootAt(_target);
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalBasicCasting::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();
	if (!_owner->getMotion()->getFrameActive())
	{
		return _status = COMPLETED;
	}
	return _status = ACTIVE;
}


//---------------------------- terminate --------------------------------------
//-----------------------------------------------------------------------------
void GoalBasicCasting::terminate()
{
	_status = COMPLETED;
	_owner->setMotionDirect("Neutral");
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalBasicCasting::render()
{
	if (Prm.RenderGoalLabel)
	{
		_goal_label->setString(
			_goal_label->getString() + "\n" + "BasicCasting");
	}
}

