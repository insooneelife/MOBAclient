#include "GoalExplore.h"
#include "../GameWorld.h"
#include "../GameMap.h"
#include "../Message/Telegram.h"
#include "../GlobalValues.h"
#include "../Util.h"
#include "../ParameterLoader.h"
#include "GoalSeekToPosition.h"
#include "GoalFollowPath.h"
#include "GoalMoveToPosition.h"



//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalExplore::activate()
{
	_status = ACTIVE;

	//if this goal is reactivated then there may be some existing subgoals that
	//must be removed
	removeAllSubgoals();

	
	//grab a random location
	_current_destination = _owner->getPathQ()->front();
	_owner->getPathQ()->push(_current_destination);
		/*util::makeRandomPosInRange(
			0, 0,
			_owner->getWorld()->getGameMap()->getSizeX(),
			_owner->getWorld()->getGameMap()->getSizeY(),
			_owner->getPos().x,
			_owner->getPos().y,
			400, 500);*/
	

	//and request a path to that position

	//the bot may have to wait a few update cycles before a path is calculated
	//so for appearances sake it simple ARRIVES at the destination until a path
	//has been found
	
	addSubgoal(
		new GoalMoveToPosition(_owner, _owner->getGoalLabel(), _current_destination));
}

//------------------------------ process -------------------------------------
//-----------------------------------------------------------------------------
int GoalExplore::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	//process the subgoals
	_status = processSubgoals();

	if (_status == COMPLETED)
		_owner->getPathQ()->pop();

	return _status;
}


//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalExplore::render()
{
	if (Prm.RenderGoalLabel)
	{
		_goal_label->setString(_goal_label->getString() + "\n" + "Explore");
	}

	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}



