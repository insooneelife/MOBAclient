#ifndef GOAL_ATTACK_TARGET_H
#define GOAL_ATTACK_TARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalAttackTarget.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------
#include "GoalComposite.h"
#include "../AbstCharacter.h"


class GoalAttackTarget : public GoalComposite<AbstCharacter>
{
public:

	GoalAttackTarget(
		AbstCharacter* pOwner,
		cocos2d::Label* const goal_label)
		:
		GoalComposite<AbstCharacter>(pOwner, GOAL_ATTACK_TARGET),
		_goal_label(goal_label)
	{}

	virtual ~GoalAttackTarget() override
	{}

	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

private:
	//for rendering (only reference owner's)
	cocos2d::Label* const	_goal_label;
};






#endif
