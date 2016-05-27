#ifndef COLLISION_AREA_H
#define COLLISION_AREA_H
//------------------------------------------------------------------------
//
//  Name:   CollisionArea.h
//
//  Desc:   Class for collision areas.
//			It has bounding shapes like circle, polygon or rectangle,
//			so it can manage collision to entity's shapes.
//			It doesn't draw pictures.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------

#include <string>
#include <sstream>
#include "BaseGameEntity.h"

class GameWorld;

class CollisionArea : public BaseGameEntity {
public:
	CollisionArea(
		GameWorld* const		world,
		geometry::Shape* const	bounding_shape,
		unsigned int			id,
		const cocos2d::Vec2&	pos,
		double radius);

	virtual ~CollisionArea() override;

	virtual void render() override;

	virtual bool handleMessage(const Telegram& msg) override;

private:
	CollisionArea(const CollisionArea&) = delete; // no copies
	CollisionArea& operator=(const CollisionArea&) = delete; // no self-assignments
	CollisionArea() = delete;
};



#endif

