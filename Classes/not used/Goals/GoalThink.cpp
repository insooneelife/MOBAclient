#include "GoalThink.h"
#include <list>

#include "GoalMoveToPosition.h"
#include "GoalExplore.h"
#include "../GlobalValues.h"
#include "GoalAttackTarget.h"
#include "GoalBasicCasting.h"

#include "../AbstCharacter.h"
#include "../Util.h"
#include "ExploreGoalEvaluator.h"
#include "AttackTargetGoalEvaluator.h"

#include "../AbstWeaponSystem.h"
#include "../CastingStrategy.h"
#include "../BaseCastingStrategy.h"

#include "GoalEnemyMove.h"

GoalThink::GoalThink(AbstCharacter* const bot, cocos2d::Label* const goal_label)
	:
	GoalComposite<AbstCharacter>(bot, GOAL_THINK),
	_goal_label(goal_label)
{

	//these biases could be loaded in from a script on a per bot basis
	//but for now we'll just give them some random values
	const double LowRangeOfBias = 0.5;
	const double HighRangeOfBias = 1.5;

	double HealthBias = util::genRand(LowRangeOfBias, HighRangeOfBias);
	double ShotgunBias = util::genRand(LowRangeOfBias, HighRangeOfBias);
	double RocketLauncherBias = util::genRand(LowRangeOfBias, HighRangeOfBias);
	double RailgunBias = util::genRand(LowRangeOfBias, HighRangeOfBias);
	double ExploreBias = util::genRand(LowRangeOfBias, HighRangeOfBias);
	double AttackBias = util::genRand(LowRangeOfBias, HighRangeOfBias);

	//create the evaluator objects
	_evaluators.emplace_back(new ExploreGoalEvaluator(ExploreBias));
	_evaluators.emplace_back(new AttackTargetGoalEvaluator(AttackBias));
}

//----------------------------- dtor ------------------------------------------
//-----------------------------------------------------------------------------
GoalThink::~GoalThink()
{}

//------------------------------- activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::activate()
{
	if (!_owner->isPossessed())
	{
		arbitrate();
	}

	_status = ACTIVE;
}

//------------------------------ process --------------------------------------
//
//  processes the subgoals
//-----------------------------------------------------------------------------
int GoalThink::process()
{
	activateIfInactive();

	int subgoal_status = processSubgoals();

	if (subgoal_status == COMPLETED || subgoal_status == FAILED)
	{
		if (!_owner->isPossessed())
		{
			_status = INACTIVE;
		}
	}

	return _status;
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::render()
{
	if (Prm.RenderGoalLabel)
	{
		_goal_label->setString("Think");
	}

	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}

//----------------------------- update ----------------------------------------
// 
//  this method iterates through each goal option to determine which one has
//  the highest desirability.
//-----------------------------------------------------------------------------
void GoalThink::arbitrate()
{
	double best = 0;
	GoalEvaluator* most_desirable = nullptr;

	//iterate through all the evaluators to see which produces the highest score
	for (auto des = std::begin(_evaluators); des != std::end(_evaluators); ++des)
	{
		double desirabilty = (*des)->calculateDesirability(_owner);

		if (desirabilty >= best)
		{
			best = desirabilty;
			most_desirable = (*des).get();
		}
	}

	CCASSERT(most_desirable, "<GoalThink::arbitrate>: no evaluator selected");

	most_desirable->setGoal(_owner);
}


//---------------------------- notPresent --------------------------------------
//
//  returns true if the goal type passed as a parameter is the same as this
//  goal or any of its subgoals
//-----------------------------------------------------------------------------
bool GoalThink::notPresent(unsigned int goal_type)const
{
	if (!_subgoals.empty())
	{
		return _subgoals.front()->getType() != goal_type;
	}

	return true;
}

void GoalThink::addGoal_MoveToPosition(cocos2d::Vec2 pos)
{
	addSubgoal(new GoalMoveToPosition(_owner, _owner->getGoalLabel(), pos));
}

void GoalThink::addGoal_BasicCasting(cocos2d::Vec2 pos)
{
	addSubgoal(new GoalBasicCasting(_owner, _owner->getGoalLabel(), pos));
}

void GoalThink::addGoal_Explore()
{
	if (notPresent(GOAL_EXPLORE))
	{
		removeAllSubgoals();
		addSubgoal(new GoalExplore(_owner, _owner->getGoalLabel()));
	}
}

/*
void GoalThink::AddGoal_GetItem(unsigned int item_type)
{
  if (notPresent(ItemTypeToGoalType(item_type)))
  {
	removeAllSubgoals();
	addSubgoal( new Goal_GetItem(_owner, item_type));
  }
}
*/

void GoalThink::addGoal_EnemyMoveToPosition()
{
	addSubgoal(new GoalEnemyMove(_owner, _owner->getGoalLabel()));
}

void GoalThink::addGoal_AttackTarget()
{
	if (notPresent(GOAL_ATTACK_TARGET))
	{
		removeAllSubgoals();
		addSubgoal(new GoalAttackTarget(_owner, _owner->getGoalLabel()));
	}
}


//-------------------------- Queue Goals --------------------------------------
//-----------------------------------------------------------------------------
void GoalThink::queueGoal_MoveToPosition(cocos2d::Vec2 pos)
{
	_subgoals.push_back(new GoalMoveToPosition(_owner, _owner->getGoalLabel(), pos));
}


