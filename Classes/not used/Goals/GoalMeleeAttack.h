#ifndef GOAL_MELEE_ATTACK_H
#define GOAL_MELEE_ATTACK_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"


class GoalMeleeAttack : public Goal<AbstCharacter>
{
public:

	GoalMeleeAttack(
		AbstCharacter* bot,
		cocos2d::Label* const goal_label);

	virtual ~GoalMeleeAttack() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

protected:
	

private:
	//for rendering (only reference owner's)
	cocos2d::Label* const		_goal_label;
};




#endif

