#ifndef HEALTH_GIVER_H
#define HEALTH_GIVER_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:     TriggerHealthGiver.h
//
//  Author:   Mat Buckland
//
//  Desc:     If a bot runs over an instance of this class its health is
//            increased. 
//
//-----------------------------------------------------------------------------
#include "Triggers/TriggerRespawning.h"
#include "../AbstCharacter.h"
#include "cocos2d.h"

class GameWorld;

class TriggerHealthGiver : public TriggerRespawning<AbstCharacter>
{
public:
	TriggerHealthGiver(
		GameWorld* const world,
		cocos2d::Layer* const view_layer,
		geometry::Shape* const bounding_shape,
		const std::string& name,
		const cocos2d::Vec2& pos,
		const cocos2d::Vec2& scale,
		double radius,
		int graph_node_index,
		int health_given,
		unsigned int respawn_delay);

	virtual ~TriggerHealthGiver() override;

	//getters && setters
	cocos2d::Sprite* 	getNowSprite() const;
	cocos2d::Label*		getLabel() const;

	//methods
	//if triggered, the bot's health will be incremented
	virtual void		actIfOverlap(AbstCharacter* bot) override;

	//virtual functions

	//draws a box with a red cross at the trigger's location
	virtual void		render() override;

private:

	//the amount of health an entity receives when it runs over this trigger
	int					_health_given;

	//for rendering
	cocos2d::Sprite*	_now_sprite;

	cocos2d::Label*		_label;
};



#endif