#include <list>
#include "BaseCharacter.h"
#include "GlobalValues.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Util.h"
#include "Message/Telegram.h"
#include "Motion.h"
#include "SteeringBehavior.h"
#include "EntityFunctionTemplates.h"
#include "SensoryMemory.h"
#include "EntityManager.h"
#include "Message/MessageDispatcher.h"
#include "MeleeTargetingSystem.h"
#include "Goals/GoalThink.h"

USING_NS_CC;

BaseCharacter::BaseCharacter(
	Layer* const			view_layer,
	const std::string&		name,
	GameWorld* const		world,
	const cocos2d::Vec2&	position,
	const cocos2d::Vec2&	margin,
	double					rotation,
	double					mass,
	double					max_force,
	double					max_speed,
	double					max_turn_rate,
	double					bounding_radius,
	int						team,
	int						max_health,
	std::queue<cocos2d::Vec2> pathQ,
	int						meleeAttackPower,
	int						gold,
	int						worth)
	:
	AbstCharacter(
		view_layer,
		name,
		world,
		position,
		margin,
		rotation,
		mass,
		max_force,
		max_speed,
		max_turn_rate,
		bounding_radius,
		meleeAttackPower,
		worth)
{
	setTeam(team); 
	setMaxHealth(max_health);
	setHealth(max_health);
	setPathQ(pathQ);
	view_layer->setCameraMask(1 << 2);

	_steering.reset(new SteeringBehavior(world, this));
	_path_planner.reset(new PathPlanner(this));
	_brain.reset(new GoalThink(this, _goal_label));
	_target_system.reset(new MeleeTargetingSystem(this));
	_sensory_memory.reset(new SensoryMemory(this, 5));

	_goal_arbitration_regulator.reset(new Regulator(4));
	_trigger_test_regulator.reset(new Regulator(8));
	_target_selection_regulator.reset(new Regulator(2));
	_vision_update_regulator.reset(new Regulator(4));

}

BaseCharacter::~BaseCharacter()
{}

void BaseCharacter::update(double time_elapsed)
{
	_brain->process();

	if (isReadyForGoalArbitration())
	{
		_brain->arbitrate();

		if (_target_selection_regulator->isReady())
		{
			_target_system->update();
		}

		//update the sensory memory with any visual stimulus
		if (_vision_update_regulator->isReady())
		{
			_sensory_memory->updateVision();
		}
	}
	updateMovement(time_elapsed);
}

bool BaseCharacter::handleMessage(const Telegram& msg)
{
	int damage = 0;
	//first see if the current goal accepts the message
	if (getBrain()->handleMessage(msg)) return true;

	//handle any messages not handles by the goals
	switch (msg.msg)
	{
	case MSG_TAKE_THAT_MF:  // 데미지 관련

		//just return if already dead or spawning
		if (isDead()) 
			return true;
		else
		{
			damage = dereferenceToType<int>(msg.extra_info);

			CCLOG("DAMAGE sender : %u, receiver : %u", msg.sender, msg.receiver);
			//the extra info field of the telegram carries the amount of damage
			//ReduceHealth(damage);

			AbstCharacter* attacker = 
				(AbstCharacter*)EntityMgr.getEntityFromID(msg.sender);
			getSensoryMemory()->updateWithDamageSource(attacker, damage);

			reduceHealth(dereferenceToType<int>(msg.extra_info));

			//if this bot is now dead let the shooter know
			if (isDead())
			{
				int worth = getWorth();
				Dispatcher.dispatchMsg(
					SEND_MSG_IMMEDIATELY,
					getId(),
					msg.sender,
					MSG_YOU_GOT_ME,
					(void*)&worth);  // 내 가치를 죽인애한테 gold로 준다.
			}

		}

		return true;
		
	case MSG_YOU_GOT_ME:  // 너가 날 죽였다.

		setScore(getScore() + 1);

		//the bot this bot has just killed should be removed as the target
		_target_system->clearTarget();

		return true;
		
	case MSG_GUN_SHOT_SOUND:

		//add the source of this sound to the bot's percepts
		getSensoryMemory()->updateWithSoundSource((AbstCharacter*)msg.extra_info);

		return true;

	case MSG_USER_HAS_REMOVED_BOT:
	{

		AbstCharacter* pRemovedBot = (AbstCharacter*)msg.extra_info;

		getSensoryMemory()->removeBotFromMemory(pRemovedBot);

		//if the removed bot is the target, make sure the target is cleared
		if (pRemovedBot == getTargetSys()->getTarget())
		{
			getTargetSys()->clearTarget();
		}

		return true;
	}


	default: return false;
	}
}


