#include "GoalMeleeAttack.h"
#include "Message/MessageDispatcher.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"
#include "../RenderUtil.h"
#include "../Motion.h"

USING_NS_CC;

//---------------------------- ctor -------------------------------------------
//-----------------------------------------------------------------------------
GoalMeleeAttack::GoalMeleeAttack(
	AbstCharacter* const bot,
	cocos2d::Label* const goal_label)
	:
	Goal<AbstCharacter>(bot, GOAL_SEEK_TO_POS),
	_goal_label(goal_label)
{}


//---------------------------- activate -------------------------------------
//-----------------------------------------------------------------------------  
void GoalMeleeAttack::activate()
{
	_status = ACTIVE;
	_owner->setVelocity(Vec2(0, 0));
	_owner->setHeading(_owner->getAttackDirection());
	
	CCLOG("ID: %u attack %u damage %d", _owner->getId(), _owner->getTargetSys()->getTarget()->getId(), _owner->getMeleeAttackPower());
	
	int d = _owner->getMeleeAttackPower();
	Dispatcher.dispatchMsg(
		SEND_MSG_IMMEDIATELY,
		_owner->getId(),
		_owner->getTargetSys()->getTarget()->getId(),
		MSG_TAKE_THAT_MF,
		(void*)&d);
	
	
	_owner->setMotionDirect("Attack1");
	_owner->getMotion()->setActive(true);
}


//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalMeleeAttack::process()
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
void GoalMeleeAttack::terminate()
{
	_status = COMPLETED;
	_owner->getMotion()->setActive(false);
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalMeleeAttack::render()
{
	if (Prm.RenderGoalLabel)
	{
		_goal_label->setString(
			_goal_label->getString() + "\n" + "MeleeAttack");
	}
}

