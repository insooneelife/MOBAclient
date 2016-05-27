#ifndef GOAL_ENEMY_MOVE_POS_H
#define GOAL_ENEMY_MOVE_POS_H
#pragma warning (disable:4786)

#include "Goals/GoalComposite.h"
#include "cocos2d.h"
#include "../AbstCharacter.h"
#include "../GlobalValues.h"


class GoalEnemyMove : public GoalComposite<AbstCharacter>
{
public:

	GoalEnemyMove(
		AbstCharacter* const	bot,
		cocos2d::Label* const	goal_label) 
		:
		GoalComposite<AbstCharacter>(bot, GOAL_MOVE_TO_POS),
		_goal_label(goal_label)
	{}
	
	GoalEnemyMove(
		AbstCharacter* const	bot,
		cocos2d::Label* const	goal_label,
		std::queue<cocos2d::Vec2>	dq)
		:
		GoalComposite<AbstCharacter>(bot, GOAL_MOVE_TO_POS),
		_goal_label(goal_label)
	{}
	virtual ~GoalEnemyMove() override
	{}

	//the usual suspects
	virtual void activate() override;

	virtual int  process() override;

	virtual void terminate() override;

	virtual void render() override;

	//this goal is able to accept messages
	virtual bool handleMessage(const Telegram& msg) override;

protected:

	//the position the bot wants to reach
	cocos2d::Vec2 _destination;



private:
	//for rendering (only reference owner's)
	cocos2d::Label* const			_goal_label;
};


#endif
