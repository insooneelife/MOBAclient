#pragma once
#include "cocos2d.h"
#include <string>
#include <map>
#include <tuple>
#include <vector>
#include "GlobalValues.h"
#include "RenderUtil.h"
#include "boost\bimap.hpp"
#include "boost\assign.hpp"

//------------------------------------------------------------------------
//
//  Name:   Motion.hpp
//
//  Desc:   
//
//  Author: Insub Im (insooneelife@naver.com)
//
//------------------------------------------------------------------------


template<typename OwnerType>
class Motion
{
public:
	enum DirectionType8
	{
		DOWN_8, LEFT_DOWN_8, LEFT_8, LEFT_UP_8,
		UP_8, RIGHT_UP_8, RIGHT_8, RIGHT_DOWN_8,
		UNABLE_8
	};

	enum DirectionType16
	{
		DOWN_16, LEFT_DOWN1_16, LEFT_DOWN2_16, LEFT_DOWN3_16,
		LEFT_16, LEFT_UP1_16, LEFT_UP2_16, LEFT_UP3_16,
		UP_16, RIGHT_UP1_16, RIGHT_UP2_16, RIGHT_UP3_16,
		RIGHT_16, RIGHT_DOWN1_16, RIGHT_DOWN2_16, RIGHT_DOWN3_16,
		UNABLE_16
	};

	typedef boost::bimaps::bimap< DirectionType8, std::string > bm_direction_type;
	static const bm_direction_type direction_types_map;

	Motion(
		const std::string&			name,
		const std::string&			foldername,
		OwnerType* const			owner,
		const cocos2d::Vec2&		pos,
		int							direction,
		int							dummy = 0);

	~Motion();

	// getters && setters

	const OwnerType* getOwner() const { return _owner; }

	int getFrame() const { return _frame; }

	void setFrame(int frame) { _frame = frame; }


	// main methods

	void update(const std::string& new_motion, const cocos2d::Vec2& new_pos, int new_dir);

	void render();

	void renderMain(const cocos2d::Vec2& new_pos, int frame);

	void renderBoundingBox();

	void renderBoundingCircle(cocos2d::Vec2 pos);

	void renderLabel(const std::string& new_motion);

private:
	Motion() = delete;
	Motion(const Motion& copy) = delete;
	Motion& operator=(const Motion& copy) = delete;

	//only for reference
	std::map<std::string, std::tuple<int, int, int> >	_motions;
	OwnerType* const									_owner;
	int													_frame;
};



#include <sstream>
#include <string>
#include "Util.h"
#include "GlobalValues.h"
#include "DebugRenderer.h"

USING_NS_CC;

template<typename OwnerType>
const typename Motion<OwnerType>::bm_direction_type Motion<OwnerType>::direction_types_map =
boost::assign::list_of<typename Motion<OwnerType>::bm_direction_type::relation >
(DOWN_8, "DOWN_8")
(LEFT_DOWN_8, "LEFT_DOWN_8")
(LEFT_8, "LEFT_8")
(LEFT_UP_8, "LEFT_UP_8")
(UP_8, "UP_8")
(RIGHT_UP_8, "RIGHT_UP_8")
(RIGHT_8, "RIGHT_8")
(RIGHT_DOWN_8, "RIGHT_DOWN_8");

template<typename OwnerType>
Motion<OwnerType>::~Motion()
{}

template<typename OwnerType>
Motion<OwnerType>::Motion(
	const std::string& name,
	const std::string& foldername,
	OwnerType* const agent,
	const Vec2& pos,
	int direction,
	int dummy)
	:
	_owner(agent),
	_frame(0)
{
	rapidjson::Document json;
	util::readJSON(foldername + "/" + name + "/" + name + ".json", json);

	for (rapidjson::Value::MemberIterator M = json["actions"].MemberBegin();
	M != json["actions"].MemberEnd(); M++)
	{
		std::string key = M->name.GetString();
		_motions.insert(std::make_pair(key,
			std::make_tuple(
				M->value["frameSize"].GetInt(),
				M->value["marginX"].GetInt(),
				M->value["marginY"].GetInt())));
	}

	auto frame_cache = SpriteFrameCache::getInstance();
	std::string filename = foldername + "/" + name + "/" + name + ".plist";
	frame_cache->addSpriteFramesWithFile(filename);

}


template<typename OwnerType>
void Motion<OwnerType>::render()
{
	renderMain(_owner->getPos(), _frame);
	renderLabel(_owner->getMotionName());
	renderBoundingBox();
}



template<typename OwnerType>
void Motion<OwnerType>::renderMain(const Vec2& new_pos, int frame)
{
	auto now = _owner->getNowSprite();
	auto shadow = _owner->getShadowSprite();

	auto frame_cache = SpriteFrameCache::getInstance();
	char number[10] = { 0 };
	sprintf(number, "_%02d.png", frame + 1);
	std::string frame_name =
		_owner->getName() + "/" + _owner->getMotionName() + "/" + number;

	auto animation_frame = frame_cache->getSpriteFrameByName(frame_name);
	now->setSpriteFrame(animation_frame);

	auto iter = _motions.find(_owner->getMotionName());
	auto data = iter->second;

	Vec2 margin(std::get<1>(data), std::get<2>(data));
	Vec2 render_pos = new_pos + margin;

	now->setGlobalZOrder(util::zOrdering(new_pos.x, new_pos.y));
	now->setPosition(render_pos);

	shadow->setSpriteFrame(animation_frame);

	float shadow_move_y = 
		(now->getBoundingBox().getMaxY() - now->getBoundingBox().getMinY()) / 4;

	shadow->setGlobalZOrder(util::zOrdering(new_pos.x, new_pos.y + shadow_move_y));
	shadow->setPosition(new_pos + Vec2(0, margin.y * shadow->getScaleY()));
}


template<typename OwnerType>
void Motion<OwnerType>::renderBoundingBox()
{
	auto bounding_box = _owner->getNowSprite()->getBoundingBox();
	float min_x = bounding_box.getMinX();
	float min_y = bounding_box.getMinY();
	float max_x = bounding_box.getMaxX();
	float max_y = bounding_box.getMaxY();

	Debug.getBoundingBoxPanel()->drawRect(
		Vec2(min_x, min_y), Vec2(max_x, max_y), cocos2d::Color4F::RED);
}


//it's time consuming for making the string
template<typename OwnerType>
void Motion<OwnerType>::renderLabel(const std::string& new_motion)
{

	/*std::stringstream ss;
	ss << _owner->getName();
	std::string str = ss.str() + "\nM : " + new_motion;
	if (_owner->getSteering().get())
	{
	auto bmap = _owner->getSteering()->behavior_types_map;
	for (auto e : bmap)
	if ((e.left & _owner->getSteering()->getBehaviorSet()) != 0x0)
	str += "\n" + e.right;

	}
	_owner->getLabel()->setString(str);*/
}

