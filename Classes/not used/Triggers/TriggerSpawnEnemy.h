#ifndef TRIGGER_SPAWN_ENEMY_H
#define TRIGGER_SPAWN_ENEMY_H
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

class TriggerSpawnEnemy : public TriggerRespawning<AbstCharacter>
{
public:
	TriggerSpawnEnemy(
		GameWorld* const world,
		cocos2d::Layer* const view_layer,
		geometry::Shape* const bounding_shape,
		const std::string& name,
		const cocos2d::Vec2& pos,
		const cocos2d::Vec2& scale,
		double radius,
		int graph_node_index,
		int team);

	virtual ~TriggerSpawnEnemy() override;

	//getters && setters
	cocos2d::Sprite* 	getNowSprite() const;
	cocos2d::Label*		getLabel() const;

	//methods
	//if triggered, the bot's health will be incremented
	virtual void		actIfOverlap(AbstCharacter* bot) override;

	//virtual functions

	virtual void		update(double time_elapsed) override;

	//draws a box with a red cross at the trigger's location
	virtual void		render() override;

private:

	//for rendering
	cocos2d::Sprite*	_now_sprite;

	cocos2d::Label*		_label;

	const cocos2d::Vec2 _pos;

	// Spawn ¼ýÀÚ
	int _numSpawn;

	cocos2d::Layer*		_view_layer;
	int _team;
};



#endif