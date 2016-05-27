#ifndef GOAL_BASIC_CASTING_H
#define GOAL_BASIC_CASTING_H
#pragma warning (disable:4786)

#include "Goal.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"


class GoalBasicCasting : public Goal<AbstCharacter>
{
public:

	GoalBasicCasting(
		AbstCharacter* bot,
		cocos2d::Label* const goal_label,
		cocos2d::Vec2 target);

	virtual ~GoalBasicCasting() override
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
	cocos2d::Vec2 _target;
};




#endif

