#include "TargetSplashSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"
#include "Message/MessageDispatcher.h"

#include "ImpactSplash.h"
#include "ActivateCollisionToTarget.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
TargetSplashSkill::TargetSplashSkill(
	cocos2d::Layer* const view_layer,
	AbstCharacter* const shooter,
	AbstCharacter* const target,
	float splash_radius)
	:
	AbstTargetSkill(
		shooter->getWorld(),
		view_layer,
		target,
		shooter->getId(),
		shooter->getPos(),
		shooter->getHeading(),
		5,
		50,
		30,
		1,
		10,
		"TargetSplashSkill",
		SKILL,
		1.5f),
	_explosion(nullptr),
	_render_once(true),
	_activated(false)
{
	CCASSERT(target, "target == nullptr");

	movePos(_heading * _bounding_radius);

	_emitter = ParticleSun::create();

	_activate_strategy.reset(new ActivateCollisionToTarget(this));
	_impact_strategy.reset(new ImpactSplash(this, splash_radius));

	_emitter->setPosition(_pos);
	_emitter->setCameraMask(1 << 2);
	_emitter->setEmissionRate(100);
	_emitter->setDuration(_survive_time);
	_emitter->setGlobalZOrder(SKILL_ZORDER);
	_emitter->setSpeed(40);
	_emitter->setTexture(Director::getInstance()->
		getTextureCache()->addImage(Prm.ParticleFolderName + "\\fire.png"));
	_emitter->setPositionType(ParticleSystem::PositionType::FREE);
	view_layer->addChild(_emitter);

	_explosion = ParticleSun::create();
	_explosion->stopSystem();

	_explosion->setPosition(_pos);
	_explosion->setCameraMask(1 << 2);
	_explosion->setEmissionRate(100);
	_explosion->setDuration(0.25f);
	_explosion->setGlobalZOrder(SKILL_ZORDER);
	_explosion->setSpeed(200);
	_explosion->setTexture(Director::getInstance()->
		getTextureCache()->addImage(Prm.ParticleFolderName + "\\fire.png"));
	_explosion->setPositionType(ParticleSystem::PositionType::FREE);

	view_layer->addChild(_explosion);
}


//------------------------------ Update ---------------------------------------
//-----------------------------------------------------------------------------
void TargetSplashSkill::update(double time_elapsed)
{
	if (Clock.getCurrentTime() > _survive_time + _time_of_creation)
	{
		_dead = true;
	}

	if (_activated)
	{
		_impact_strategy->impact();
	}
	else
	{
		_activated = _activate_strategy->test();

		updateMovement(time_elapsed);
	}
}

void TargetSplashSkill::updateMovement(double time_elapsed)
{

	_velocity = getMaxSpeed() * (_target->getPos() - _pos).getNormalized();

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
void TargetSplashSkill::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	Debug.getBoundingCirclePanel()->
		drawCircle(getPos(), getBRadius(), (float)(2 * M_PI), 50, false, Color4F::WHITE);

	_emitter->setPosition(getPos());

	if (_activated)
	{
		if (_render_once)
		{
			_emitter->stopSystem();
			_explosion->resetSystem();
			_render_once = false;
		}
		_explosion->setPosition(getPos());
	}
}
