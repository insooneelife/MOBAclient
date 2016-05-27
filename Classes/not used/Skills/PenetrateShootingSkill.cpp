#include "PenetrateShootingSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"
#include "Message/MessageDispatcher.h"

#include "ImpactNonRepeatSplash.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
PenetrateShootingSkill::PenetrateShootingSkill(
	cocos2d::Layer* const view_layer,
	AbstCharacter* const shooter,
	Vec2 target_pos)
	:
	AbstNonTargetSkill(
		shooter->getWorld(),
		view_layer,
		target_pos,
		shooter->getId(),
		shooter->getPos(),
		shooter->getHeading(),
		5,
		50,
		20,
		2,
		10,
		"PenetrateShootingSkill",
		SKILL,
		1.5f)
{
	CCASSERT(target_pos != Vec2(), "target_pos != Vec2()");

	movePos(_heading * _bounding_radius * 2);

	_impact_strategy.reset(new ImpactNonRepeatSplash(this, _bounding_radius));

	_emitter = ParticleSystemQuad::create(Prm.ParticleFolderName + "\\FireSpear.plist");
	_emitter->setPosition(_pos);
	_emitter->setRotation(- util::getAngleFromZero(shooter->getHeading()));
	_emitter->setCameraMask(1 << 2);
	_emitter->setEmissionRate(150);
	_emitter->setDuration(_survive_time);
	_emitter->setGlobalZOrder(SKILL_ZORDER);
	_emitter->setSpeed(10);
	_emitter->setTexture(Director::getInstance()->
		getTextureCache()->addImage(Prm.ParticleFolderName + "\\fire.png"));
	_emitter->setPositionType(ParticleSystem::PositionType::FREE);
	view_layer->addChild(_emitter);
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void PenetrateShootingSkill::update(double time_elapsed)
{
	if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}

	updateMovement(time_elapsed);

	_impact_strategy->impact();
}

void PenetrateShootingSkill::updateMovement(double time_elapsed)
{
	_velocity = getMaxSpeed() * getHeading();

	//make sure vehicle does not exceed maximum velocity
	if (_velocity.getLength() > _max_speed)
	{
		_velocity.normalize();
		_velocity *= _max_speed;
	}

	movePos(_velocity);
}


//-------------------------- render -------------------------------------------
//-----------------------------------------------------------------------------
void PenetrateShootingSkill::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	Debug.getBoundingCirclePanel()->
		drawCircle(getPos(), getBRadius(), (float)(2 * M_PI), 50, false, Color4F::WHITE);

	_emitter->setPosition(getPos());

}
