#include "CollisionArea.h"
#include "Util.h"
#include "RenderUtil.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "DebugRenderer.h"

USING_NS_CC;

CollisionArea::CollisionArea(
	GameWorld* const		world,
	geometry::Shape* const	bounding_shape,
	unsigned int			id,
	const cocos2d::Vec2&	pos,
	double radius)
	: 
	BaseGameEntity(world, bounding_shape, id, STRUCTURE, "", radius, pos, Vec2(0, 0))
{}

CollisionArea::~CollisionArea()
{}

void CollisionArea::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());
}

bool CollisionArea::handleMessage(const Telegram& msg)
{
	return false;
}