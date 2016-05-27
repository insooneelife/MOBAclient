#ifndef GOAL_EXPLORE_H
#define GOAL_EXPLORE_H
#pragma warning (disable:4786)

#include "Goals/GoalComposite.h"
#include "../GlobalValues.h"
#include "cocos2d.h"


class AbstCharacter;


class GoalExplore : public GoalComposite<AbstCharacter>
{
public:

	GoalExplore(
		AbstCharacter* const owner,
		cocos2d::Label* const goal_label)
		:
		GoalComposite<AbstCharacter>(owner, GOAL_EXPLORE),
		_goal_label(goal_label),
		_destination_is_set(false)
	{}

	virtual ~GoalExplore() override
	{}

	virtual void activate() override;

	virtual int process() override;

	virtual void terminate() override
	{}

	virtual void render() override;

protected:

	cocos2d::Vec2  _current_destination;

	//set to true when the destination for the exploration has been established
	bool      _destination_is_set;

private:
	//for rendering (only reference owner's)
	cocos2d::Label* const	_goal_label;
};


#endif
