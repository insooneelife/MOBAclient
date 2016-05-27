#include "ImpactSplash.h"
#include "../CollisionTestTemplates.h"
#include "../AbstCharacter.h"
#include "../Message/MessageDispatcher.h"

ImpactSplash::ImpactSplash(
	AbstSkill* const owner_skill,
	float splash_radius)
	:
	ImpactStrategy(owner_skill),
	_splash_radius(splash_radius),
	_impacted(false)
{}


//----------------------------- impact ------------------------------------
//
//  This iterate through agent(character) cellspaces and give messages 
//  to them with damage info if they overlap.
//-------------------------------------------------------------------------

void ImpactSplash::impact()
{
	if (_impacted)
		return;

	_impacted = true;

	auto hit_list = getListOfCollisionEntitiesCellSpace(
		_owner_skill, _owner_skill->getWorld()->getGameMap()->getAgentCellSpace().get());

	if (hit_list.size() > 0)
	{
		for (auto hit : hit_list)
		{
			int damage = _owner_skill->getDamage();

			//send a message to the bot to let it know it's been hit, and who the
			//shot came from.
			Dispatcher.dispatchMsg(
				SEND_MSG_IMMEDIATELY,
				_owner_skill->getId(),
				hit->getId(),
				MSG_TAKE_THAT_MF,
				(void*)&damage);
		}
	}
}
