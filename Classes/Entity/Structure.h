#ifndef STRUCTURE_H
#define STRUCTURE_H
//------------------------------------------------------------------------
//
//  Name:   Structure.h
//
//  Desc:   Class for structures.
//			It has bounding shapes like circle, polygon or rectangle,
//			but it doesn't check collision with others.
//			It draws pictures.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <string>
#include <sstream>
#include "BaseGameEntity.h"

class GameWorld;

class Structure : public BaseGameEntity 
{
public:
	Structure(
		GameWorld* const		world,
		cocos2d::Layer* const	view_layer,
		geometry::Shape* const	bounding_shape,
		unsigned int			id,
		const std::string&		name,
		const cocos2d::Vec2&	pos,
		const cocos2d::Vec2&	margin,
		const cocos2d::Vec2&	shadow_margin,
		double radius,
		bool render_shadow);

	virtual ~Structure() override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& msg) override;

protected:
	//for rendering
	cocos2d::Sprite*		_now_sprite;

	cocos2d::Sprite*		_shadow_sprite;

private:
	Structure(const BaseGameEntity&) = delete; // no copies
	Structure& operator=(const Structure&) = delete; // no self-assignments
	Structure() = delete;
};



#endif

