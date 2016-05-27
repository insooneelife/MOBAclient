#include <list>
#include "UserCharacter.h"
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
#include "AbstWeaponSystem.h"

USING_NS_CC;

UserCharacter::UserCharacter(
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
	_steering.reset(new SteeringBehavior(world, this));
	_path_planner.reset(new PathPlanner(this));
	_brain.reset(new GoalThink(this, _goal_label));
	_target_system.reset(new MeleeTargetingSystem(this));
	_sensory_memory.reset(new SensoryMemory(this, 5));

	_weapon_system.reset(new AbstWeaponSystem(this, 0.2, 0.1, 1));

	_goal_arbitration_regulator.reset(new Regulator(4));
	_trigger_test_regulator.reset(new Regulator(8));
	_target_selection_regulator.reset(new Regulator(2));
	_vision_update_regulator.reset(new Regulator(4));
}

UserCharacter::~UserCharacter()
{}

void UserCharacter::update(double time_elapsed)
{
	_brain->process();

	updateMovement(time_elapsed);
}

bool UserCharacter::handleMessage(const Telegram& msg)
{
	int damage = 0;
	//first see if the current goal accepts the message
	if (getBrain()->handleMessage(msg)) return true;

	//handle any messages not handles by the goals
	switch (msg.msg)
	{
	case MSG_TAKE_THAT_MF:

		//just return if already dead or spawning
		if (isDead())
			return true;
		else
		{
			damage = dereferenceToType<int>(msg.extra_info);

			//the extra info field of the telegram carries the amount of damage
		
			// 아직 유저가 죽는것 안함.
		//	reduceHealth(dereferenceToType<int>(msg.extra_info));

			AbstCharacter* attacker =
				(AbstCharacter*)EntityMgr.getEntityFromID(msg.sender);
			getSensoryMemory()->updateWithDamageSource(attacker, damage);

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

	case MSG_YOU_GOT_ME:  // 내가 누굴 죽였다고 메시지를 받음

		setScore(getScore() + 1);

		// gold 추가
		addGold(dereferenceToType<int>(msg.extra_info));

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

//---------------------------- fireWeapon -------------------------------------
//
//  fires the current weapon at the given position
//-----------------------------------------------------------------------------
void UserCharacter::fireWeapon(Vec2 pos)
{
	_weapon_system->shootAt(pos);
}
