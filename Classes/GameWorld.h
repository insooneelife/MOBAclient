#ifndef GameWorld_H
#define GameWorld_H
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   GameWorld.h
//
//  Desc:   All the environment data and methods for the game 
//			entity behaviors. This class is the root of the project's
//          update and render calls.
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------


#include <map>
#include <memory>
#include "cocos2d.h"

class BaseGameEntity;
class Character;
class GameMap;
class Skill;

class GameWorld 
{
public:
	typedef std::map<unsigned int, std::unique_ptr<Character> > CharacterMap;
	typedef std::map<unsigned int, std::unique_ptr<Skill> > SkillMap;

	enum { SKILL_NON, SKILL_Q, SKILL_W, SKILL_E, SKILL_R, ATTACK_A };

	explicit GameWorld(cocos2d::Layer* const view_layer, const float& x, const float& y);

	~GameWorld();

	const CharacterMap&					getCharacters() const;

	const SkillMap&						getSkills() const;

	const std::unique_ptr<GameMap>&		getGameMap() const;

	int									getCurrentSkillKey() const;

	void								setCurrentSkillKey(int new_key);

	unsigned int						getCurrentClickedEntityID() const;

	void								setCurrentClickedEntityID(unsigned int id);



	void update(float time_elapsed);

	void render();

	void addCharacter(
		unsigned int id,
		const std::string& name,
		float x, float y,
		float mx, float my,
		float bradius);

	void addUserCharacter(
		unsigned int id,
		const std::string& cname,
		float x, float y,
		float mx, float my,
		float bradius,
		const std::string& client_name);

	void addSkill(
		unsigned int id,
		const std::string& name,
		float x, float y,
		float mx, float my,
		float bradius);

	void deleteCharacter(unsigned int id);

	void deleteSkill(unsigned int id);

	void clickRightMouseButton(cocos2d::Vec2 p);

	void clickLeftMouseButton(cocos2d::Vec2 p);

	void releaseLeftMouseButton();

	void moveMouse(cocos2d::Vec2 p);

	void animationOnWorld(
		const std::string& name,
		const cocos2d::Vec2& pos);

	void soundEffectOnWorld(
		const std::string& name,
		const cocos2d::Vec2& pos);


private:
	GameWorld(const GameWorld&) = delete; // no copies
	GameWorld& operator=(const GameWorld&) = delete; // no self-assignments
	GameWorld() = delete;

	std::unique_ptr<GameMap>		_map;

	//a container of all the moving entities
	CharacterMap					_characters;

	SkillMap						_skills;

	unsigned int					_current_clicked_entity_ID;

	// 현재 활성화된 스킬 enum
	int								_current_skill_key;

	//references the game entity view layer
	cocos2d::Layer* const 			_view_layer;

	//references the camera view position
	const float&					_camera_x;
	const float&					_camera_y;

	std::map<std::string, std::tuple<unsigned int, float> > _global_animations;
};



#endif