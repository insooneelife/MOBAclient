#ifndef SKILL_H
#define SKILL_H
#pragma warning (disable:4786)

//-----------------------------------------------------------------------------
//
//  Name:   Skill.h
//
//  Author: Insub Im (insooneelife@naver.com)
//
//  Desc:   
//
//-----------------------------------------------------------------------------

#include "cocos2d.h"
#include "BaseGameEntity.h"

class Skill : public BaseGameEntity
{
public:

	Skill(cocos2d::Layer* const	view_layer,
		GameWorld* const		world,
		unsigned int			id,
		const std::string&		name,
		const cocos2d::Vec2&	position,
		const cocos2d::Vec2&	margin,
		double					bounding_radius);

	virtual ~Skill() override {}

	void update(cocos2d::Vec2 pos, bool activated);

	virtual void render() override;

	virtual bool handleMessage(const Telegram& telegram) override
	{
		return false;
	}

private:

	bool _activated;

	cocos2d::ParticleSystemQuad* _explosion;

	bool _render_once;

	//is it dead? A dead collision skill is one that has come to the end of its
	//trajectory and cycled through any explosion sequence. A dead skill effect
	//can be removed from the world environment and deleted.
	bool				_dead;

	//this is stamped with the time this collision effect was instantiated.
	//This is to enable the shot to be rendered for a specific length of time
	float				_time_of_creation;

	float				_survive_time;

	//how much damage the skill inflicts
	int					_damage;


	bool _emitter_flag;
	bool _explosion_flag;

	//for rendering
	cocos2d::ParticleSystemQuad* _emitter;
};


#endif