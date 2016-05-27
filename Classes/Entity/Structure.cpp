#include "Structure.h"
#include "Util.h"
#include "RenderUtil.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "DebugRenderer.h"

USING_NS_CC;

Structure::Structure(
	GameWorld* const		world,
	cocos2d::Layer* const	view_layer,
	geometry::Shape* const	bounding_shape,
	unsigned int			id,
	const std::string&		name,
	const cocos2d::Vec2&	pos,
	const cocos2d::Vec2&	margin,
	const cocos2d::Vec2&	shadow_margin,
	double radius,
	bool render_shadow)
	: 
	BaseGameEntity(
		world,
		bounding_shape,
		id,
		STRUCTURE,
		name,
		radius,
		pos - margin,
		margin),
	_now_sprite(nullptr),
	_shadow_sprite(nullptr)
{
	std::string foldername = Prm.StructureFolderName + "/" + name + ".png";
	Vec2 render_pos = _pos + margin;

	_now_sprite = Sprite::create(foldername);
	_now_sprite->setCameraMask(1 << 2);
	_now_sprite->setPosition(render_pos);
	_now_sprite->setGlobalZOrder(util::zOrdering(_pos.x, _pos.y));
	_now_sprite->setLocalZOrder(util::zOrdering(_pos.x, _pos.y));



	if (render_shadow)
	{
		_shadow_sprite = Sprite::create();
		_shadow_sprite->setCameraMask(1 << 2);

		RenderUtil::shadowSetting(
			view_layer,
			_now_sprite,
			_shadow_sprite,
			_pos);

		_shadow_sprite->setPosition(_pos + shadow_margin);
	}

	view_layer->addChild(_now_sprite);
}

Structure::~Structure()
{
	//_now_sprite must be deleted after child renderers.

	if(_shadow_sprite)
		_shadow_sprite->removeFromParent();
	if(_now_sprite)
		_now_sprite->removeFromParent();
}

void Structure::render()
{
	//render for debug
	_bounding_shape->render(Debug.getBoundingShapePanel());

	float min_x = _now_sprite->getBoundingBox().getMinX();
	float min_y = _now_sprite->getBoundingBox().getMinY();
	float max_x = _now_sprite->getBoundingBox().getMaxX();
	float max_y = _now_sprite->getBoundingBox().getMaxY();

	Debug.getBoundingBoxPanel()->drawRect(
		Vec2(min_x, min_y), Vec2(max_x, max_y), cocos2d::Color4F::RED);
}

bool Structure::handleMessage(const Telegram& msg)
{
	return false;
}