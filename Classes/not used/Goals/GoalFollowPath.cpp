#include "GoalFollowPath.h"
#include "GoalTraverseEdge.h"
#include "../AbstCharacter.h"
#include "../GameWorld.h"
#include "../DebugRenderer.h"

USING_NS_CC;

//------------------------------ ctor -----------------------------------------
//-----------------------------------------------------------------------------
GoalFollowPath::GoalFollowPath(
	AbstCharacter* const bot,
	cocos2d::Label* const goal_label,
	std::list<PathEdge> path) 
	:
	GoalComposite<AbstCharacter>(bot, GOAL_FOLLOW_PATH),
	_goal_label(goal_label),
	_path(path)
{}


//------------------------------ activate -------------------------------------
//-----------------------------------------------------------------------------
void GoalFollowPath::activate()
{
	_status = ACTIVE;

	//get a reference to the next edge
	PathEdge edge = _path.front();

	//remove the edge from the path
	_path.pop_front();

	//some edges specify that the bot should use a specific behavior when
	//following them. This switch statement queries the edge behavior flag and
	//adds the appropriate goals/s to the subgoal list.

	switch (edge.getBehavior())
	{
	case NavGraphEdge::NORMAL:
	{
		addSubgoal(new GoalTraverseEdge(
			_owner,
			_owner->getGoalLabel(),
			edge,
			_path.empty()));
	}

	break;

	case NavGraphEdge::GOES_THROUGH_DOOR:
	{

		//also add a goal that is able to handle opening the door
		//addSubgoal(new Goal_NegotiateDoor(_owner, edge, _path.empty()));
	}

	break;

	case NavGraphEdge::JUMP:
	{
		//add subgoal to JUMP along the edge
	}

	break;

	case NavGraphEdge::GRAPPLE:
	{
		//add subgoal to GRAPPLE along the edge
	}

	break;

	default:

		throw std::runtime_error("<GoalFollowPath::activate>: Unrecognized edge type");
	}
}


//-------------------------- process ------------------------------------------
//-----------------------------------------------------------------------------
int GoalFollowPath::process()
{
	//if status is INACTIVE, call activate()
	activateIfInactive();

	_status = processSubgoals();
	//if there are no subgoals present check to see if the path still has edges.
	//remaining. If it does then call activate to grab the next edge.
	if (_status == COMPLETED && !_path.empty())
	{
		activate();
	}

	return _status;
}

void GoalFollowPath::terminate()
{
	_owner->setMotionDirect("Neutral");
	_owner->setVelocity(Vec2(0, 0));
}

//---------------------------- render -----------------------------------------
//-----------------------------------------------------------------------------
void GoalFollowPath::render()
{
	if (Prm.RenderPathLine)
	{
		for (auto e : _path)
		{
			Debug.getPathLinePanel()->
				drawLine(e.getSource(), e.getDestination(), cocos2d::Color4F::BLUE);

			Debug.getPathLinePanel()->
				drawCircle(
					e.getDestination(),
					10,
					(float)(2 * M_PI),
					40, false, Color4F::GREEN);
		}
	}

	if (Prm.RenderGoalLabel)
	{
		_goal_label->setString(_goal_label->getString() + "\n" + "FollowPath");
	}

	//forward the request to the subgoals
	GoalComposite<AbstCharacter>::render();
}



