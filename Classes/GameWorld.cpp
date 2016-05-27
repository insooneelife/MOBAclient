#include <list>
#include "GameWorld.h"
#include "Util.h"
#include "GlobalValues.h"
#include "cocos2d.h"
#include "EntityManager.h"
#include "ParameterLoader.h"
#include "AppMacros.h"
#include "GameMap.h"
#include "Entity\Character.h"
#include "Entity\UserCharacter.h"
#include "Entity\Skill.h"
 
#include "Network\GameClient\GameClientManager.h"
#include "Network\Packet\PacketFactory.h"

#include "Motion.hpp"
#include "Database.h"

#include "AudioEngine.h"


#include <atomic>

using std::list;
USING_NS_CC;


//------------------------------- ctor -----------------------------------
//------------------------------------------------------------------------
GameWorld::GameWorld(Layer* const view_layer, const float& x, const float& y)
	:
	_map(new GameMap(this, view_layer)),
	_view_layer(view_layer),
	_current_clicked_entity_ID(0),
	_current_skill_key(SKILL_NON),
	_camera_x(x),
	_camera_y(y)
{
	ParameterLoader::getInstance();
	PacketFactory::getInstance().setGameWorld(this);
	DB;
	CCLOG("ctor GameWorld()");

	// read jsons about global animations
	rapidjson::Document json;
	util::readJSON("animations/animations.json", json);

	for (rapidjson::Value::ValueIterator it = json.Begin(); it != json.End(); it++)
	{
		std::string value = it->GetString();

		rapidjson::Document json2;
		util::readJSON("animations/" + value +".json", json2);

		std::string name = json2.FindMember("name")->value.GetString();
		unsigned int size = json2.FindMember("size")->value.GetInt();
		float delay = json2.FindMember("delay")->value.GetDouble();

		_global_animations.insert(std::make_pair(name, std::make_tuple(size, delay)));
	}
}

//-------------------------------- dtor ----------------------------------
//------------------------------------------------------------------------
GameWorld::~GameWorld() 
{
	CCLOG("~GameWorld()");
	PacketFactory::getInstance().setGameWorld(nullptr);
}


//----------------------------- update -----------------------------------
//------------------------------------------------------------------------
void GameWorld::update(float time_elapsed) 
{
	// loop for process client read messages
	GameMessage temp_msg;
	while (PacketFactory::getInstance().getQ().pop(temp_msg))
	{
		PacketFactory::getInstance().decodeByType(temp_msg);
	}
}

void GameWorld::render() 
{
	//render any skills
	for (auto sk = std::begin(_skills); sk != std::end(_skills); ++sk)
	{
		sk->second->render();
	}

	//render characters
	for (auto ch = std::begin(_characters); ch != std::end(_characters); ch++)
	{
		ch->second->render();
	}

	_map->render();
	
}

void GameWorld::addCharacter(
	unsigned int id,
	const std::string& name,
	float x, 
	float y,
	float mx, 
	float my,
	float bradius)
{
	auto iter = _characters.find(id);
	if (iter != std::end(_characters))
	{
		CCLOG("already exist id!!");
		return;
	}

	_characters.insert(
		std::make_pair(
			id,
			std::unique_ptr<Character>(
				new Character(
				_view_layer,
				this,
				id,
				name,
				Vec2(x, y),
				Vec2(mx, my),
				bradius))));
}

void GameWorld::addUserCharacter(
	unsigned int id,
	const std::string& cname,
	float x,
	float y,
	float mx,
	float my,
	float bradius,
	const std::string& client_name)
{
	auto iter = _characters.find(id);
	if (iter != std::end(_characters))
	{
		CCLOG("already exist id!!");
		return;
	}

	_characters.insert(
		std::make_pair(
			id,
			std::unique_ptr<UserCharacter>(
				new UserCharacter(
					_view_layer,
					this,
					id,
					cname,
					Vec2(x, y),
					Vec2(mx, my),
					bradius,
					client_name))));
}

void GameWorld::addSkill(
	unsigned int id,
	const std::string& name,
	float x,
	float y,
	float mx,
	float my,
	float bradius)
{
	_skills.insert(
		std::make_pair(
			id,
			std::unique_ptr<Skill>(
				new Skill(
					_view_layer,
					this,
					id,
					name,
					Vec2(x, y),
					Vec2(mx, my),
					bradius))));
}

void GameWorld::deleteCharacter(unsigned int id)
{
	auto iter = _characters.find(id);
	if (iter != std::end(_characters))
	{
		_characters.erase(iter);
	}
}

void GameWorld::deleteSkill(unsigned int id)
{
	auto iter = _skills.find(id);
	if (iter != std::end(_skills))
	{
		_skills.erase(iter);
	}
}

void GameWorld::clickRightMouseButton(cocos2d::Vec2 p)
{
	GameClientManager::getInstance().send(
		PacketFactory::getInstance().createPacketClick(
			GameClientManager::getInstance().getGameClient()->getId(),
			p.x, p.y, packets::MouseButton_RIGHT, 0, _current_clicked_entity_ID));

	// click rendering
	animationOnWorld("right_mouse_click", p);
}

// this function is for mouse release actions
// such as returning the color of entity
void GameWorld::releaseLeftMouseButton()
{
	auto iter = _characters.find(_current_clicked_entity_ID);
	if (iter != std::end(_characters))
	{
		iter->second->getNowSprite()->setColor(cocos2d::Color3B::WHITE);
	}
}

void GameWorld::clickLeftMouseButton(cocos2d::Vec2 p)
{
	// give server the packet of mouse left click
	GameClientManager::getInstance().send(
		PacketFactory::getInstance().createPacketClick(
			GameClientManager::getInstance().getGameClient()->getId(),
			p.x, p.y, packets::MouseButton_LEFT, _current_skill_key, _current_clicked_entity_ID));

	_current_skill_key = SKILL_NON;

	// click rendering
	animationOnWorld("left_mouse_click", p);
	
}

void GameWorld::moveMouse(cocos2d::Vec2 p)
{
	//auto pinfo = AutoPolygon::generatePolygon(filename);
	//_polygonSprite = Sprite::create(pinfo);
}

void GameWorld::animationOnWorld(
	const std::string& name,
	const cocos2d::Vec2& p)
{
	auto sprite = Sprite::create();
	
	size_t size = 0;
	float delay = 0;
	auto iter = _global_animations.find(name);
	if (iter != std::end(_global_animations))
	{
		size = std::get<0>(iter->second);
		delay = std::get<1>(iter->second);
	}

	sprite->setPosition(p);
	sprite->setGlobalZOrder(ANIMATION_ZORDER);
	
	//plist 정보를 SpriteFrameCache에 저장합니다.
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(name + ".plist");

	//spriteFrame에 대한 정보가 모두 있기 때문에 cache에서 불러와 애니메이션에 추가만 해주시면 됩니다.
	Animation* animation = Animation::create();
	animation->setDelayPerUnit(delay);

	for (size_t i = 1; i <= size; i++)
	{
		char number[10] = { 0 };
		sprintf(number, "_%02d.png", i);
		auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(name + number);
		animation->addSpriteFrame(frame);
	}

	sprite->setCameraMask(1 << 2);

	auto anim = Animate::create(animation);
	auto actionRemove = RemoveSelf::create();
	auto seq = Sequence::create(anim, actionRemove, NULL);

	sprite->runAction(seq);
	_view_layer->addChild(sprite);
}

void GameWorld::soundEffectOnWorld(
	const std::string& name,
	const cocos2d::Vec2& pos)
{
	if (util::inRange(Vec2(_camera_x, _camera_y), pos, Prm.SoundEffectRange))
	{
		float volume = 1.0f - Vec2(_camera_x, _camera_y).getDistance(pos) / Prm.SoundEffectRange;

		experimental::AudioEngine::play2d(name + ".mp3", false, volume);
	}
}


const GameWorld::CharacterMap& GameWorld::getCharacters() const
{
	return _characters;
}

const GameWorld::SkillMap& GameWorld::getSkills() const
{
	return _skills;
}

const std::unique_ptr<GameMap>& GameWorld::getGameMap() const
{
	return _map;
}

int GameWorld::getCurrentSkillKey() const
{
	return _current_skill_key;
}

void GameWorld::setCurrentSkillKey(int new_key)
{
	_current_skill_key = new_key;
}

unsigned int GameWorld::getCurrentClickedEntityID() const
{
	return _current_clicked_entity_ID;
}

void GameWorld::setCurrentClickedEntityID(unsigned int id)
{
	_current_clicked_entity_ID = id;
}