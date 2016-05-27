#ifndef ABSTWEAPONSYSTEM
#define ABSTWEAPONSYSTEM
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:	AbstWeaponSystem.h
//
//  Author:	Insub Im (insooneelife@naver.com)
//
//  Desc:   class to manage all different kind of spell action.
//
//-----------------------------------------------------------------------------

#include <map>
#include <memory>
#include "cocos2d.h"

class AbstCharacter;
class CastingStrategy;


class AbstWeaponSystem
{
protected:

	//a map of weapon instances indexed into by type
	typedef std::map<int, std::unique_ptr<CastingStrategy> >  WeaponMap;

	AbstCharacter* const	_owner;

	//pointers to the weapons the bot is carrying 
	//(a bot may only carry one instance of each weapon)
	WeaponMap		_weapon_map;

	//a pointer to the weapon the bot is currently holding
	CastingStrategy*	_current_weapon;

	//this is the minimum amount of time a bot needs to see an opponent before
	//it can react to it. This variable is used to prevent a bot shooting at
	//an opponent the instant it becomes visible.
	double			_reaction_time;

	//each time the current weapon is fired a certain amount of random noise is
	//added to the the angle of the shot. This prevents the bots from hitting
	//their opponents 100% of the time. The lower this value the more accurate
	//a bot's aim will be. Recommended values are between 0 and 0.2 (the value
	//represents the max deviation in radians that can be added to each shot).
	double			_aim_accuracy;

	//the amount of time a bot will continue aiming at the position of the target
	//even if the target disappears from view.
	double			_aim_persistance;

	//predicts where the target will be by the time it takes the current weapon's
	//projectile type to reach it. Used by takeAimAndShoot
	cocos2d::Vec2    predictFuturePositionOfTarget()const;

	//adds a random deviation to the firing angle not greater than _aim_accuracy 
	//rads
	void addNoiseToAim(cocos2d::Vec2& aiming_pos)const;

public:

	AbstWeaponSystem(
		AbstCharacter* owner,
		double      reactionTime,
		double      aim_accuracy,
		double      aim_persistance);

	~AbstWeaponSystem();

	//sets up the weapon map with just one weapon: the blaster
	virtual void	initialize();

	//this method aims the bot's current weapon at the target 
	//(if there is a target) and, if aimed correctly, fires a round. (Called each update-step
	//from Raven_Bot::Update)
	virtual void	takeAimAndShoot()const;

	//this method determines the most appropriate weapon to use given the current
	//game state. (Called every n update-steps from Raven_Bot::Update)
	virtual void	selectWeapon();

	//changes the current weapon to one of the specified type 
	//(provided that type is in the bot's possession)
	void			changeWeapon(unsigned int type);

	//shoots the current weapon at the given position
	void			shootAt(cocos2d::Vec2 pos) const;

	//returns a pointer to the current weapon
	CastingStrategy*	getCurrentWeapon() const { return _current_weapon; }

	//returns a pointer to the specified weapon type 
	//(if in inventory, null if not)
	CastingStrategy*	getWeaponFromInventory(int weapon_type);

	double			reactionTime() const { return _reaction_time; }

};

#endif