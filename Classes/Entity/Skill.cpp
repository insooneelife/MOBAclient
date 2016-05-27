#include <tuple>
#include "Skill.h"
#include "GameWorld.h"
#include "GameMap.h"
#include "Message/MessageDispatcher.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"
#include "ParameterLoader.h"
#include "Database.h"


USING_NS_CC;

Skill::Skill(
	Layer* const         view_layer,
	GameWorld* const      world,
	unsigned int         id,
	const std::string&      name,
	const cocos2d::Vec2&   position,
	const cocos2d::Vec2&   margin,
	double               bounding_radius)
	:
	BaseGameEntity(
		world,
		new geometry::Circle(position, bounding_radius),
		id,
		SKILL,
		name,
		bounding_radius,
		position,
		margin),
	_explosion(nullptr),
	_render_once(true),
	_activated(true)
{
	// sound
	world->soundEffectOnWorld(name, position);

	auto data = DB.getSkillData()[name];

	std::string type = std::get<0>(data);
	int damage = std::get<1>(data);
	int mana = std::get<2>(data);
	int radius = std::get<3>(data);
	int speed = std::get<4>(data);
	_emitter_flag = std::get<5>(data);
	_explosion_flag = std::get<6>(data);

	int period = std::get<7>(data);
	int splash_radius = std::get<8>(data);
	double survive_time = std::get<9>(data);

	int emitter_emission_rate = std::get<10>(data);
	double emitter_duration = std::get<11>(data);
	double emitter_scale = std::get<12>(data);
	int emitter_speed = std::get<13>(data);

	int explosion_emission_rate = std::get<14>(data);
	double explosion_duration = std::get<15>(data);
	double explosion_scale = std::get<16>(data);
	int explosion_speed = std::get<17>(data);

	std::string filename = "skills\\" + name + "\\" + name;
	//std::string filename = name;

	_emitter = ParticleSystemQuad::create(filename + ".plist");
	_explosion = ParticleSystemQuad::create(filename + "_exp.plist");

	if (name == "FireCircle")
	{
		Vec2 p;
		float r = 40;
		p = getPos();

		p.x += r;

		//_emitter = ParticleSystemQuad::create(filename + ".plist");
		_emitter->setPosition(p);
		//_emitter->setRotation(-util::getAngleFromZero(shooter->getHeading()));
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(5);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);

		p = getPos();
		p.x -= r;
		//_emitter = ParticleSystemQuad::create(filename + ".plist");
		_emitter->setPosition(p);
		//_emitter->setRotation(-util::getAngleFromZero(shooter->getHeading()));
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(5);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);

		p = getPos();
		p.x -= r / 2;
		p.y += (r / 2) * sqrt(3);
		//_emitter = ParticleSystemQuad::create(filename + ".plist");
		_emitter->setPosition(p);
		//_emitter->setRotation(-util::getAngleFromZero(shooter->getHeading()));
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(emitter_speed);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);

		p = getPos();
		p.x += r / 2;
		p.y += (r / 2) * sqrt(3);
		_emitter->setPosition(p);
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(emitter_speed);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);

		p = getPos();
		p.x -= r / 2;
		p.y -= (r / 2) * sqrt(3);
		_emitter->setPosition(p);
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(emitter_speed);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);

		p = getPos();
		p.x += r / 2;
		p.y -= (r / 2) * sqrt(3);
		_emitter->setPosition(p);
		_emitter->setCameraMask(1 << 2);
		_emitter->setEmissionRate(emitter_emission_rate);
		_emitter->setDuration(emitter_duration);
		_emitter->setGlobalZOrder(((1999999999) - (p.y * (18000) + p.x)));
		_emitter->setSpeed(emitter_speed);
		_emitter->setTexture(Director::getInstance()->
			getTextureCache()->addImage(filename + ".png"));
		_emitter->setPositionType(ParticleSystem::PositionType::FREE);
		view_layer->addChild(_emitter);
	}
	else
	{
		if (_emitter_flag)
		{
			_emitter->setTexture(Director::getInstance()->
				getTextureCache()->addImage(filename + ".png"));

			_emitter->setPosition(_pos);
			_emitter->setCameraMask(1 << 2);
			_emitter->setEmissionRate(emitter_emission_rate);
			_emitter->setDuration(emitter_duration);
			_emitter->setGlobalZOrder(((1999999999) - (getPos().y * (18000) + getPos().x)));
			_emitter->setScale(emitter_scale);
			_emitter->setSpeed(emitter_speed);
			_emitter->stopSystem();

			_emitter->setPositionType(ParticleSystem::PositionType::FREE);
			view_layer->addChild(_emitter);
		}

		if (_explosion_flag)
		{
			_explosion->setTexture(Director::getInstance()->
				getTextureCache()->addImage(filename + ".png"));
			_explosion->stopSystem();

			_explosion->setScale(explosion_scale);
			_explosion->setPosition(_pos);
			_explosion->setCameraMask(1 << 2);
			_explosion->setEmissionRate(explosion_emission_rate);
			_explosion->setDuration(explosion_duration);
			_explosion->setGlobalZOrder(((1999999999) - (getPos().y * (18000) + getPos().x)));
			_explosion->setSpeed(explosion_speed);
			_explosion->setPositionType(ParticleSystem::PositionType::FREE);

			view_layer->addChild(_explosion);
		}
	}
}


void Skill::update(cocos2d::Vec2 pos, bool activated)
{
	setPos(pos);
	_activated = activated;
}

void Skill::render()
{
	_bounding_shape->render(Debug.getBoundingShapePanel());

	Debug.getCustomShapePanel()->
		drawCircle(getPos(), getBRadius(), (float)(2 * M_PI), 50, false, Color4F::WHITE);

	if (_emitter_flag)
	{
		_emitter->setPosition(getPos());
		//_emitter->resetSystem();
	}

	if (_activated)
	{
		if (_render_once)
		{
			_emitter->resetSystem();
			//if (_emitter_flag)
			//   _emitter->stopSystem();
			if (_explosion_flag)
				_explosion->resetSystem();
			_render_once = false;
		}
		if (_explosion_flag)
			_explosion->setPosition(getPos());
	}
	//_explosion->stopSystem();
}