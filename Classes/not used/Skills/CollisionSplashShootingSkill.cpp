#include "CollisionSplashShootingSkill.h"
#include "AbstCharacter.h"
#include "GameWorld.h"
#include "CollisionTestTemplates.h"
#include "GameMap.h"
#include "Message/MessageDispatcher.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"
#include "ParameterLoader.h"

#include "ActivateCollisionToAny.h"
#include "ImpactSplash.h"

USING_NS_CC;

//-------------------------- ctor ---------------------------------------------
//-----------------------------------------------------------------------------
CollisionSplashShootingSkill::CollisionSplashShootingSkill(
	cocos2d::Layer* const view_layer,
	AbstCharacter* const shooter,
	Vec2 target,
	float splash_radius)
	:
	AbstNonTargetSkill(
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
		"CollisionSplashShootingSkill",
		SKILL,
		1.5f),
	_explosion(nullptr),
	_render_once(true),
	_activated(false)
{
	CCASSERT(target != Vec2(), "target != Vec2()");

	movePos(_heading * _bounding_radius);

	_emitter = ParticleSun::create();
	//_emitter = ParticleGalaxy::create();
	//_emitter = ParticleMeteor::create();
	//_emitter->setRotation(135-util::getAngleFromZero(shooter->getHeading()));
	//_emitter = ParticleFire::create();
	//_emitter->setRotation(90 - util::getAngleFromZero(shooter->getHeading()));

	_activate_strategy.reset(new ActivateCollisionToAny(this));
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
void CollisionSplashShootingSkill::update(double time_elapsed)
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

void CollisionSplashShootingSkill::updateMovement(double time_elapsed)
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
void CollisionSplashShootingSkill::render()
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


