#include "GoalEnemyMove.h"
#include "../AbstCharacter.h"
#include "../GameMap.h"
#include "../Message/Telegram.h"

#include "GoalSeekToPosition.h"
#include "GoalFollowPath.h"



//------------------------------- activate ------------------------------------
//-----------------------------------------------------------------------------
void GoalEnemyMove::activate()
{
	_status = ACTIVE;
	//make sure the subgoal list is clear.
	removeAllSubgoals();

	//requests a path to the target position from the path planner. Because, for
	//demonstration purposes, the Raven path planner uses time-slicing when 
	//processing the path requests the bot may have to wait a few update cycles
	//before a path is calculated. Consequently, for appearances sake, it just
	//seeks directly to the target position whilst it's awaiting notification
	//that the path planning request has succeeded/FAILED
	
	if (!_owner->getPathQ()->empty())
		_destination = _owner->getPathQ()->front();
	
	
	if (_owner->getPathPlanner()->requestPathToPosition(_destination))
	{
		//CCLOG("DESTINATION: %lf, %lf\n", _destination.x, _destination.y);
		addSubgoal(new GoalSeekToPosition(
			_owner,
			_owner->getGoalLabel(),
			_destination));
	}
}

//------------------------------ process --------------------------------------
//-----------------------------------------------------------------------------
int GoalEnemyMove::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();
	if (_status == COMPLETED && !_owner->getPathQ()->empty())
	{
		_status = INACTIVE;
		_destination = _owner->getPathQ()->front();
		_owner->getPathQ()->pop();
	}

	//if any of the subgoals have FAILED then this goal re-plans
	reactivateIfFailed();

	return _status;
}

// terminate
void GoalEnemyMove::terminate()
{
	removeAllSubgoals();
	_status = COMPLETED;
	//CCLOG("MOVETO END!!!!\n");
}

//---------------------------- handleMessage ----------------------------------
//-----------------------------------------------------------------------------
bool GoalEnemyMove::handleMessage(const Telegram& msg)
{
	//first, pass the message down the goal hierarchy
	bool bHandled = forwardMessageToFrontMostSubgoal(msg);
	// CR: deleted four lines containing pathPlanningTime
	//     see RavenPlanner::cycleOnce

	//if the msg was not handled, test to see if this goal can handle it
	if (bHandled == false)
	{
		switch (msg.msg)
		{
		case MSG_PATH_READY:

		  //clear any existing goals
			removeAllSubgoals();

			addSubgoal(new GoalFollowPath(
				_owner,
				_owner->getGoalLabel(),
				_owner->getPathPlanner()->getPath()));

			return true; //msg handled


		case MSG_NOPATH_AVAILABLE:

			_status = FAILED;

			return true; //msg handled

		default: return false;
		}
	}

	//handled by subgoals
	return true;
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalEnemyMove::render()
{
	_goal_label->setString(_goal_label->getString() + "\n" + "GoalEnemyMove");

	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}