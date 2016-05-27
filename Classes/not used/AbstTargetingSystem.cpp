#include "AbstTargetingSystem.h"
#include "AbstCharacter.h"
#include "SensoryMemory.h"

USING_NS_CC;

//-------------------------------- ctor ---------------------------------------
//-----------------------------------------------------------------------------
AbstTargetingSystem::AbstTargetingSystem(AbstCharacter* const owner)
	:
	_owner(owner),
	_current_target(nullptr)
{}

bool AbstTargetingSystem::isTargetWithinFOV() const
{
  return _owner->getSensoryMemory()->isOpponentWithinFOV(_current_target);
}

//returns true if there is a currently assigned target
bool AbstTargetingSystem::isTargetPresent() const 
{
	return _current_target != nullptr;
}

bool AbstTargetingSystem::isTargetAttackable() const
{
  return _owner->getSensoryMemory()->isOpponentAttackable(_current_target);
}

cocos2d::Vec2 AbstTargetingSystem::getLastRecordedPosition() const
{
  return _owner->getSensoryMemory()->getLastRecordedPositionOfOpponent(_current_target);
}

double AbstTargetingSystem::getTimeTargetHasBeenVisible() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenVisible(_current_target);
}

double AbstTargetingSystem::getTimeTargetHasBeenOutOfView() const
{
  return _owner->getSensoryMemory()->getTimeOpponentHasBeenOutOfView(_current_target);
}

//returns a pointer to the target. null if no target current.
AbstCharacter* AbstTargetingSystem::getTarget() const 
{
	return _current_target; 
}

//sets the target pointer to null
void AbstTargetingSystem::clearTarget() 
{
	_current_target = nullptr; 
}