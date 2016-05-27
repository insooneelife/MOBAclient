#include "BaseCastingStrategy.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "GameMap.h"

USING_NS_CC;

//--------------------------- ctor --------------------------------------------
//-----------------------------------------------------------------------------
BaseCastingStrategy::BaseCastingStrategy(AbstCharacter*   owner) 
	:
	CastingStrategy(
		BaseGameEntity::TYPE_ROCKET_LAUNCHER,
		1.5,
		150,
		50,
		owner)
{}


//------------------------------ shootAt --------------------------------------
//-----------------------------------------------------------------------------
void BaseCastingStrategy::shootAt(Vec2 pos)
{
	if (isReadyForNextShot())
	{
		//fire off a rocket!
		_owner->getWorld()->AddRocket(_owner, pos);

		updateTimeWeaponIsNextAvailable();

		//add a trigger to the game so that the other bots can hear this shot
		//(provided they are within range)
		//_owner->getWorld()->getGameMap()->addSoundTrigger(_owner, 400);
	}
}



//-------------------------------- render -------------------------------------
//-----------------------------------------------------------------------------
void BaseCastingStrategy::render()
{
}