#ifndef GOAL_HUNT_TARGET_H
#define GOAL_HUNT_TARGET_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   GoalHuntTarget.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   Causes a bot to search for its current target. Exits when target
//          is in view
//-----------------------------------------------------------------------------
#include "GoalComposite.h"
#include "../AbstCharacter.h"


class GoalHuntTarget : public GoalComposite<AbstCharacter>
{
protected:

	//this value is set to true if the last visible position of the target
	//bot has been searched without success
	bool  _last_visit_visible_pos_tried;

public:

	GoalHuntTarget(
		AbstCharacter* const bot,
		cocos2d::Label* const goal_label)
		:
		GoalComposite<AbstCharacter>(bot, GOAL_HUNT_TARGET),
		_last_visit_visible_pos_tried(false),
		_goal_label(goal_label)
	{}

	virtual ~GoalHuntTarget() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override
	{}

	virtual void render() override;

private:
	//for rendering (only reference owner's)
	cocos2d::Label* const	_goal_label;
};


#endif